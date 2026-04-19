#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <filesystem>

#include "types.hpp"

namespace ConfPrsr {
	void parse(const std::filesystem::path& path, Config& conf);
}

#endif
