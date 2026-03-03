#include <vector>
#include <memory>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "constants.hpp"
#include "event.hpp"
#include "utilities.hpp"


int main() {
	std::vector<std::unique_ptr<DesktopFile>> as_structs;
	Event event { as_structs };

	Utils::collect_all_df(as_structs, dirs_to_search);
	Utils::prepare_space();

	event.event_loop(update_freq);

	Utils::clean_space();

    return 0;
}
