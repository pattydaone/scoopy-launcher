// This file is primarily used for finder debugging
// since its a little bit more difficult to do while 
// in an "interactive" environment

#include <cstddef>
#include <vector>
#include <memory>
#include <iostream>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "constants.hpp"
#include "event.hpp"
#include "finder.hpp"
#include "utilities.hpp"
#include "config_parser.hpp"

std::vector<std::unique_ptr<DesktopFile>> as_structs;
std::vector<std::string> dirs_to_search { home_dir + "/.local/share" };

int main() {
	Config config {};
	if (std::filesystem::exists(home_dir + "/.config/scoopy-launcher/config")) {
		ConfPrsr::parse(home_dir + "/.config/scoopy-launcher/config", config);
	}
	else if (std::filesystem::exists(home_dir + "/.scoopy-launcher")) {
		ConfPrsr::parse(home_dir + "/.scoopy-launcher", config);
	}
	
	std::vector<std::unique_ptr<DesktopFile>> as_structs;
	Event event { as_structs, config };

	if (std::getenv("XDG_DATA_DIRS")) {
		std::string data_dirs { std::getenv("XDG_DATA_DIRS") };
		Utils::split(data_dirs, dirs_to_search, ':');
	}
	else {
		dirs_to_search.push_back("/usr/share");
		dirs_to_search.push_back("/usr/local/share");
	}

	Utils::collect_all_df(as_structs, dirs_to_search);
	Utils::prepare_space(config);

	event.event_loop(update_freq);

	Utils::clean_space();
    return 0;
}
