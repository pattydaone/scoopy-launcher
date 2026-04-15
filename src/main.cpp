#include <vector>
#include <memory>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "constants.hpp"
#include "event.hpp"
#include "utilities.hpp"

std::vector<std::string> dirs_to_search { home_dir + "/.local/share" };

int main() {
	std::vector<std::unique_ptr<DesktopFile>> as_structs;
	Event event { as_structs };

	if (std::getenv("XDG_DATA_DIRS")) {
		std::string data_dirs { std::getenv("XDG_DATA_DIRS") };
		Utils::split(data_dirs, dirs_to_search, ':');
	}
	else {
		dirs_to_search.push_back("/usr/share");
		dirs_to_search.push_back("/usr/local/share");
	}

	Utils::collect_all_df(as_structs, dirs_to_search);
	Utils::prepare_space();

	event.event_loop(update_freq);

	Utils::clean_space();

    return 0;
}
