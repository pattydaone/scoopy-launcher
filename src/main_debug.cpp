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

std::vector<std::unique_ptr<DesktopFile>> as_structs;

int main() {
	Utils::collect_all_df(as_structs, dirs_to_search);
    return 0;
}
