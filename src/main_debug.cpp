// This file is primarily used for finder debugging
// since its a little bit more difficult to do while 
// in an "interactive" environment

#include <vector>
#include <memory>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "constants.hpp"
#include "event.hpp"
#include "finder.hpp"
#include "utilities.hpp"

std::vector<std::unique_ptr<DesktopFile>> as_structs;

int main() {

	Utils::collect_all_df(as_structs, dirs_to_search);

	for (auto& i : as_structs) {
		if (i->Name.contains("esktop")) {
			fuzzyfind(i, "vesktop");
		}
		else if (i->Name.contains("icinae")) {
			fuzzyfind(i, "vesktop");
		}
	}

    return 0;
}
