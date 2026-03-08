#include <cstddef>
#include <ctime>
#include <iostream>
#include <ostream>
#include <thread>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "event.hpp"
#include "ascii.hpp"
#include "finder.hpp"
#include "utilities.hpp"

Event::Event(std::vector<std::unique_ptr<DesktopFile>>& as_structs)
	: df_files { as_structs }, exit_proc { false }, selected_line(3) {
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	event_queue.push_back(Events::redraw_screen);
}

void Event::event_loop(int frequency) {
	while (!exit_proc) {
		check_input();
		if (!event_queue.empty()) process_events();
		std::this_thread::sleep_for(std::chrono::milliseconds(frequency));
	}
}

void Event::add_event(Events event) {
	event_queue.push_back(event);
}

void Event::process_events() {
	for (Events i : event_queue) {
		switch (i) {
			case (Events::exit): 
				exit_proc = true;
				break;
			case (Events::highlight): // consider removing entirely
				redraw_screen();
				break;
			case (Events::launch):
				launch();
				break;
			case (Events::redraw_screen):
				redraw_screen();
				break;
			case (Events::find):
				 find();
				break;
			case (Events::Null):
				break;
		}
	}
	event_queue.clear();
}

void Event::launch() {
	df_files[selected_line - 3]->saved_match_score = 10;
	std::string exec { df_files[selected_line - 3]->Exec };
	std::size_t percent = exec.find("%");
	if (percent != std::string::npos) --percent; // Remove trailing space
	pid_t pid = fork();
	if (pid < 0) {
		Utils::clean_space();
		std::cout << "fork failed :(" << std::flush;
		exit(1);
	}
	else if (pid == 0) {
		setsid();
		setpgid(pid, pid);
		std::vector<std::string> arg_list = Utils::get_args(exec.substr(0,percent));
		std::size_t arg_num = arg_list.size();
		const char** args = new const char* [arg_num + 1];

		for (std::size_t i = 0; i < arg_num; ++i) {
			args[i] = arg_list[i].c_str();
		}
		args[arg_num] = NULL;
		int fd = open("/dev/null", O_WRONLY);
		dup2(fd, STDIN_FILENO); // surely redirecting stdin to a write only fd isnt a bad idea !
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		execvp(args[0], (char**)args);
		close(fd);
		delete[] args;
	}
	else {
		exit_proc = true;
	}
}

void Event::redraw_screen() {
	using namespace Ascii;
	std::cout << esc << save_pos << std::flush;

	std::cout << beginning_rows_down(2) << std::flush;
	if (actual_out.length() == 0) {
		for (std::size_t i = 0; i < df_files.size(); ++i) {
			if (static_cast<int>(i) == size.ws_row - 3){
				printed_entries = i;
				break;
			}
			if (static_cast<int>(i + 3) == selected_line) {
				std::cout << change_foreground_color(ForegroundColors::Magenta) << std::flush;
				std::cout << " >" << df_files[i]->Name << std::flush;
				std::cout << change_foreground_color() << std::flush;
				std::cout << beginning_rows_down(1) << std::flush;
				continue;
			}
			std::cout << esc << erase_line << std::flush;
			std::cout << "▎ " << df_files[i]->Name << std::flush;
			std::cout << beginning_rows_down(1) << std::flush;
		}
	}
	else {
		for (std::size_t i = 0; i < df_files.size(); ++i) {
			if (df_files[i]->match_score <= 0 || static_cast<int>(i) == size.ws_row - 3) {
				std::cout << esc << erase_to_end_screen << std::flush;
				printed_entries = i;
				break;
			}
			std::cout << esc << erase_line << std::flush;
			if (static_cast<int>(i + 3) == selected_line) {
				std::cout << change_foreground_color(ForegroundColors::Magenta);
				std::cout << " >" << df_files[i]->Name << std::flush;
				std::cout << change_foreground_color() << std::flush;
				std::cout << beginning_rows_down(1) << std::flush;
				continue;
			}
			std::cout << "▎ " << df_files[i]->Name << std::flush;
			std::cout << beginning_rows_down(1) << std::flush;
		}
	}
	std::cout << esc << load_pos << std::flush;
}

void Event::find() {
	std::for_each(df_files.begin(), df_files.end(), 
			[this](std::unique_ptr<DesktopFile>& df) { fuzzyfind(df, actual_out); });
	std::sort(df_files.begin(), df_files.end(), df_gt);
	redraw_screen();
}

void Event::backspace_pressed() {
	using namespace Ascii;
	if (actual_out.length() > 0) {
		actual_out.pop_back();
		std::cout << move_left(1) << std::flush;
		std::cout << esc << erase_to_end_line << std::flush;
		add_event(Events::find);
	}
}

void Event::get_input() {
	input.clear();
	char buf[1024];
	ssize_t count = 0;
	while ((count = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		input.append(std::string_view(buf, count));
	}
}

void Event::check_input() {
	using namespace Ascii;
	get_input();
	
	if (input.length() > 0) {
		for (auto i : input) {
			if (i == esc_key) {
				add_event(Events::exit);
				return;
			}

			else if (i == backspace) {
				backspace_pressed();
				return;
			}

			else if (i == tab) {
				++selected_line;
				if (selected_line > static_cast<int>(printed_entries + 2)) selected_line = 3;
				add_event(Events::redraw_screen);
				return;
			}

			else if (i == enter) {
				add_event(Events::launch);
				return;
			}
			actual_out += i;
			std::cout << i << std::flush;
		}
		add_event(Events::find);
	}
}
