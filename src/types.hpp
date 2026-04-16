#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

#include "ascii.hpp"

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
	ForegroundColors selected_background_color = ForegroundColors::Magenta;
	bool select_with_foreground = true;
	BackgroundColors selected_foreground_color = BackgroundColors::Default;
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
};

#endif
