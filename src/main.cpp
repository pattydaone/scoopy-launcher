#include <vector>
#include <memory>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <filesystem>

#include "config_parser.hpp"
#include "constants.hpp"
#include "event.hpp"
#include "types.hpp"
#include "utilities.hpp"

std::vector<std::string> dirs_to_search { home_dir + "/.local/share" };

int main(int argc, char** argv) {
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
	Utils::prepare_space();

	event.event_loop(update_freq);

	Utils::clean_space();

    return 0;
}
