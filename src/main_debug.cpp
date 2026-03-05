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
	std::string exec {};
	for (auto& i : as_structs) {
		if (i->Name.contains("ULTRAKILL")) {
			exec = i->Exec;
		}
	}
	std::size_t percent = exec.find("%");
	if (percent != std::string::npos) --percent;
	pid_t pid = fork();
	if (pid < 0) {
		// TODO: error
	}
	else if (pid == 0) {
		setsid();
		std::vector<std::string> arg_list = Utils::get_args(exec.substr(0,percent));
		std::size_t arg_num = arg_list.size();
		const char** args = new const char* [arg_num + 1];

		for (std::size_t i = 0; i < arg_num; ++i) {
			args[i] = arg_list[i].c_str();
		}
		args[arg_num] = NULL;

		// int fd = open("/dev/null", O_WRONLY);
		// dup2(fd, STDOUT_FILENO);
		// dup2(fd, STDERR_FILENO);
		execvp(args[0], (char**)args);
		// close(fd);
		delete[] args;
	}
	else {
		return 0;
	}
    return 0;
}
