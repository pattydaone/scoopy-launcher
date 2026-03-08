#include <iostream>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include <termios.h>
#include <unistd.h>

#include "ascii.hpp"
#include "utilities.hpp"
#include "desktop_file_parser.hpp"
#include "types.hpp"

namespace Utils {
	using namespace Ascii;
	void get_desktop_files(const std::array<std::string, 2>& dirs, std::vector<std::string>& file_paths) {
		for (const auto& dir_path : dirs) {
			for (auto dir_entry : std::filesystem::directory_iterator{ dir_path }) {
				file_paths.push_back(dir_entry.path());
			}
		}
	}

	void split_actions(std::string_view actions, std::vector<std::string_view>& out_vec) {
		std::size_t pos { 0 };

		while ((pos = actions.find(";", 0)) != std::string_view::npos) {
			out_vec.emplace_back(actions.substr(0, pos));
			actions.remove_prefix(pos + 1);
		}
		out_vec.emplace_back(actions);
	}

	void set_action_knowns(DesktopFile& action, const std::unique_ptr<DesktopFile>& poi) {
		action.Comment = poi->Comment;
		action.Type = poi->Type;
		action.Icon = poi->Icon;
		action.Categories = poi->Categories;
		action.Keywords = poi->Keywords;
	}

	void collect_all_df(std::vector<std::unique_ptr<DesktopFile>>& as_structs, const std::array<std::string, 2>& dirs_to_search) {
		std::vector<std::string> desktop_file_paths;
		get_desktop_files(dirs_to_search, desktop_file_paths);

		for (auto& path : desktop_file_paths) {
			if (!path.ends_with(".desktop")) { continue; }
			as_structs.push_back(Prsr::get_desktop_struct_pointer(path));
			if (as_structs.back()->Actions.length()) Prsr::append_actions(path, as_structs);
		}
	}

	bool DfGt::operator()(const std::unique_ptr<DesktopFile>& df1, const std::unique_ptr<DesktopFile>& df2) {
		return df1->match_score > df2->match_score;
	}

	bool prepare_terminal(bool on) {
		struct termios settings;
		if (tcgetattr(STDIN_FILENO, &settings)) return false;
		if (on) {
			// Disable canonical mode
			settings.c_lflag &= ~(ICANON);
			settings.c_cc[VMIN] = 0;
			settings.c_cc[VTIME] = 0;
			// Disable echo
			settings.c_lflag &= ~(ECHO);
			// Translate return to nl
			settings.c_iflag |= ICRNL;
		}
		else {
			// Enable canonical mode
			settings.c_lflag |= ICANON;
			// Enable echo
			settings.c_lflag |= ECHO;
			// Don't translate return to nl
			settings.c_iflag &= ~(ICRNL);
		}
		return 0 == tcsetattr(STDIN_FILENO, TCSANOW, &settings);
	}

	void prepare_space() {
		prepare_terminal(true);

		struct winsize size;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

		std::cout << esc << enable_buff << esc << erase_screen << std::flush;
		std::cout << esc << home << std::flush;

		std::cout << go_to(2, 0);
		for (int i = 0; i < size.ws_col; ++i) { std::cout << "━"; }
		std::cout << esc << home << "> " << std::flush;
	}

	void clean_space() {
		std::cout << esc << erase_screen << esc << home << std::flush;
		prepare_terminal(false);
	}

	const char* remove_trailing_space(std::string& inp) {
		if (inp.back() == ' ') inp.pop_back();
		return inp.c_str();
	}

	std::vector<std::string> get_args(const std::string& exec) {
		std::string_view exec_view = exec;
		std::vector<std::string> arg_list {};
		std::size_t pos { 0 };
		while ((pos = exec_view.find(" ", 0)) != std::string_view::npos) {
			arg_list.emplace_back(exec_view.substr(0, pos));
			exec_view.remove_prefix(pos + 1);
		}
		arg_list.emplace_back(exec_view);

		return arg_list;
	}
}
