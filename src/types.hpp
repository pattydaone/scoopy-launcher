#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

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

#endif
