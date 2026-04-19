#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

#include "ascii.hpp"
#include "exceptions.hpp"

enum class DesktopFileEntries {
    Null = 0,
    Name,
    Comment,
    Exec,
    Icon,
    Type,
    Actions
};

struct DesktopFile {
    std::string Name {};
    std::string Comment {};
    std::string Exec {};
    std::string Icon {};
    std::string Type {};
    std::string Actions {};
	std::string Keywords {};
	std::string Categories {};
	bool terminal = false;
	int match_score = 0;
	int match_offset = 0;

    void set_entry(const std::string& key, const std::string& value) {
		if (key == "Name") Name = value;
		else if (key == "Comment")    Comment = value;
		else if (key == "Exec")       Exec = value;
		else if (key == "Icon")       Icon = value;
		else if (key == "Type")       Type = value;
		else if (key == "Actions")    Actions = value;
		else if (key == "Keywords")   Keywords = value;
		else if (key == "Categories") Categories = value;
		else if (key == "Terminal")   terminal = (value[0] == 't' ? true : false);
    }
};

enum class Events {
	Null = 0,
	redraw_screen,
	highlight,
	exit,
	launch,
	find
};

// Comments will be provided, for now, for myself.
struct Config {
	/* The color of the selected entry, and whether that should be foreground, 
	 * background, or both */
	ForegroundColors selected_foreground_color = ForegroundColors::Magenta;
	bool select_with_foreground = true;
	BackgroundColors selected_background_color = BackgroundColors::Default;
	bool select_with_background = false;
	
	ForegroundColors global_foreground_color = ForegroundColors::Default;
	BackgroundColors global_background_color = BackgroundColors::Default;

	bool remove_duplicates = true; // desktop file entries with duplicate names... or exec, haven't decided yet
	bool filter_nameless = true; // desktop file entries where the name var is unset
	bool search_comment = true; // Whether to fuzzy find within comments 
	bool search_keywords = true; // whether to fuzzy find within keywords
	bool search_categories = false; // you get it

	bool print_comment = true; // whether to print out the comment to screen 
	int cycle_down_key; // Consider string? idk, sounds hard to do...
	int cycle_up_key; 
	int select_key;
	int exit_key;

	std::string default_terminal_command { "xdg-terminal" }; // If the user doesn't have or doesn't want xdg-terminal
	bool open_in_new_terminal = true; // terminal=true applications open in the same terminal as the launcher

	void set_entry(const std::string& key, const std::string& value) {
		if (key == "selection_background_color") { selected_background_color = static_cast<BackgroundColors>(std::stoi(value)); }
		else if (key == "selection_foreground_color") { selected_foreground_color = static_cast<ForegroundColors>(std::stoi(value)); }
		else if (key == "select_with") {
			if (value == "foreground") {
				select_with_foreground = true;
				select_with_background = false;
			}
			else if (value == "background") {
				select_with_foreground = false;
				select_with_background = true;
			}
			else {
				throw(ConfigError(value));
			}
		}
		else if (key == "background_color") { global_background_color = static_cast<BackgroundColors>(std::stoi(value)); }
		else if (key == "foreground_color") { global_foreground_color = static_cast<ForegroundColors>(std::stoi(value)); }
		else if (key == "remove_duplicates") { remove_duplicates = (value[0] == 't' ? true : false); }
		else if (key == "filter_nameless")   { filter_nameless   = (value[0] == 't' ? true : false); }
		else if (key == "search_comment")    { search_comment    = (value[0] == 't' ? true : false); }
		else if (key == "search_keywords")   { search_keywords   = (value[0] == 't' ? true : false); }
		else if (key == "search_categories") { search_categories = (value[0] == 't' ? true : false); }
		else if (key == "print_comment")     { print_comment     = (value[0] == 't' ? true : false); }
		else if (key == "cycle_down") {}
		else if (key == "cycle_up") {}
		else if (key == "select") {}
		else if (key == "exit") {}
		else if (key == "default_terminal_command") { default_terminal_command = value; }
		else if (key == "open_in_new_terminal") { open_in_new_terminal = (value[0] == 't' ? true : false); }
		else {
			throw(ConfigError(value));
		}
	}
};

#endif
