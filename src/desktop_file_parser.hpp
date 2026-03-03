#ifndef DESKTOP_FILE_PARSER_HPP
#define DESKTOP_FILE_PARSER_HPP

#include <memory>
#include <vector>

#include "types.hpp"

namespace Prsr {
	std::unique_ptr<DesktopFile> get_desktop_struct_pointer(const std::string& path);

	void append_actions(const std::string& path, std::vector<std::unique_ptr<DesktopFile>>& desktop_structs);
}

#endif
