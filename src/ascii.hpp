#ifndef ASCII_HPP
#define ASCII_HPP

#include <iostream>
#include <sstream>
#include <string_view>

enum class ForegroundColors {
	Black     = 30,
	B_Black   = 90,
	Red       = 31,
	B_Red     = 91,
	Green     = 32,
	B_Green   = 92,
	Yellow    = 33,
	B_Yellow  = 93,
	Blue      = 34,
	B_Blue    = 94,
	Magenta   = 35,
	B_Magenta = 95,
	Cyan      = 36,
	B_Cyan    = 96,
	White     = 37,
	B_White   = 97,
	Default   = 39,
	Reset     =  0
};

enum class BackgroundColors {
	Black     =  40,
	B_Black   = 100,
	Red       =  41,
	B_Red     = 101,
	Green     =  42,
	B_Green   = 102,
	Yellow    =  43,
	B_Yellow  = 103,
	Blue      =  44,
	B_Blue    = 104,
	Magenta   =  45,
	B_Magenta = 105,
	Cyan      =  46,
	B_Cyan    = 106,
	White     =  47,
	B_White   = 107,
	Default   =  49,
	Reset     =   0
};

namespace Ascii {
	using namespace std::string_view_literals;

	static constexpr auto esc = "\033"sv;
	static constexpr auto enable_buff = "[?1049h"sv;
	static constexpr auto disable_buff = "[?1049l"sv;
	static constexpr auto save_pos = "7"sv;
	static constexpr auto load_pos = "8"sv;
	static constexpr auto erase_line = "[2K"sv;
	static constexpr auto erase_to_end_screen = "[0J"sv;
	static constexpr auto erase_to_end_line = "[0K"sv;
	static constexpr auto erase_screen = "[2J"sv;
	static constexpr auto home = "[H"sv;

	static constexpr int esc_key = 27;
	static constexpr int backspace = 127;
	static constexpr int tab = 9;
	static constexpr char enter = '\n';

	inline std::string change_foreground_color(ForegroundColors color = ForegroundColors::Default) {
		std::stringstream ret;
		ret << esc << "[" << static_cast<int>(color) << "m";
		return ret.str();
	}

	inline std::string change_background_color(BackgroundColors color = BackgroundColors::Default) {
		std::stringstream ret;
		ret << esc << "[" << static_cast<int>(color) << "m";
		return ret.str();
	}

	inline std::string go_to(int row, int column) {
		std::stringstream ret;
		ret << esc << "[" << row << ";" << column << "H";
		return ret.str();
	}

	inline std::string beginning_rows_down(int rows) {
		std::stringstream ret;
		ret << esc << "[" << rows << "E";
		return ret.str();
	}

	inline std::string move_left(int cols) {
		std::stringstream ret;
		ret << esc << "[" << cols << "D";
		return ret.str();
	}
}

#endif // !ASCII_HPP
