#include <cstddef>
#include <ctime>
#include <iostream>
#include <ostream>
#include <sys/ioctl.h>
#include <thread>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "event.hpp"
#include "ascii.hpp"
#include "finder.hpp"
#include "types.hpp"
#include "utilities.hpp"

Event::Event(std::vector<std::unique_ptr<DesktopFile>>& as_structs, Config& conf)
	: df_files { as_structs }, exit_proc { false }, selected_line(3), conf { conf } {
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	event_queue.push_back(Events::resize);
	event_queue.push_back(Events::redraw_screen);
}

void Event::event_loop(int frequency) {
	while (!exit_proc) {
		check_input();
		check_resize();
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
			case (Events::resize):
				resize();
				break;
			case (Events::Null):
				break;
		}
	}
	event_queue.clear();
}

void Event::launch() {
	std::string exec { df_files[selected_line - 3]->Exec };
	bool terminal = df_files[selected_line - 3]->terminal;
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
		std::vector<std::string> arg_list = Utils::get_args(exec.substr(0,percent), terminal, conf);
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

void Event::print_line(const std::unique_ptr<DesktopFile>& df, bool selected) {
	using namespace Ascii;
	int printed_width = 2 + df->Name.length() + 2 + df->Comment.length();

	std::cout << esc << erase_line << std::flush;
	if (selected) {
		std::cout << (conf.select_with_foreground ? 
						change_foreground_color(conf.selected_foreground_color) : change_background_color(conf.selected_background_color)) << std::flush;
		std::cout << " >" << df->Name;
		if (conf.print_comment) {
			if (printed_width > size.ws_col) {
				std::cout << ": " << df->Comment.substr(0, size.ws_col - df->Name.length() - 7) << "...";
			}
			else {
				std::cout << ": " << df->Comment;
			}
		}
		std::cout << std::flush;
		std::cout << change_foreground_color() << std::flush;
		std::cout << beginning_rows_down(1) << std::flush;
		return;
	}
	
	std::cout << "▎ " << df->Name;
	if (conf.print_comment) {
		if (printed_width > size.ws_col) {
			std::cout << ": " << df->Comment.substr(0, size.ws_col - df->Name.length() - 7) << "...";
		}
		else {
			std::cout << ": " << df->Comment;
		}
	}
	std::cout << beginning_rows_down(1);
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
				print_line(df_files[i], true);
				continue;
			}

			print_line(df_files[i], false);
		}
	}
	else {
		for (std::size_t i = 0; i < df_files.size(); ++i) {
			if (df_files[i]->match_score <= 0 || static_cast<int>(i) == size.ws_row - 3) {
				std::cout << esc << erase_to_end_screen << std::flush;
				printed_entries = i;
				break;
			}

			if (static_cast<int>(i + 3) == selected_line) {
				print_line(df_files[i], true);
				continue;
			}
			print_line(df_files[i], false);
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

void Event::resize() {
	using namespace Ascii;
	std::cout << esc << home << std::flush;
	std::cout << esc << erase_line << std::flush;
	std::cout << actual_out << std::flush;
	std::cout << esc << save_pos << std::flush;

	std::cout << go_to(2, 0) << std::flush;;
	for (int i = 0; i < size.ws_col; ++i) { std::cout << "━"; }
	std::cout << esc << load_pos << std::flush;
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

void Event::check_resize() {
	struct winsize cur_size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &cur_size);
	if (cur_size.ws_col != size.ws_col || cur_size.ws_row != size.ws_row) {
		size = cur_size;
		add_event(Events::resize);
	}
}
