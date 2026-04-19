#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>
#include <memory>
#include <string>

#include <sys/ioctl.h>

#include "types.hpp"
#include "utilities.hpp"

class Event {
	Utils::DfGt df_gt;
	std::vector<std::unique_ptr<DesktopFile>>& df_files;
	std::vector<Events> event_queue;
	bool exit_proc;
	std::string input;
	std::string actual_out;
	int selected_line;
	struct winsize size;
	std::size_t printed_entries;
	Config conf;

	void process_events();
	void redraw_screen();
	void launch();
	void find();

	void backspace_pressed();
	void get_input();
	void check_input();
public:
	Event(std::vector<std::unique_ptr<DesktopFile>>& as_structs, Config& conf);
	void event_loop(int frequency);
	void add_event(Events event);
};

#endif // !EVENT_HPP
