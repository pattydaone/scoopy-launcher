#include <fstream>

#include "config_parser.hpp"

void ConfPrsr::parse(const std::filesystem::path &path, Config &conf) {
	std::ifstream conf_file { path };
	std::string line {};
	std::string key {};
	std::string value {};
	std::size_t equals;

	while (std::getline(conf_file, line)) {
		equals = line.find_first_of('=');
		if (equals == std::string::npos) continue;
		key = line.substr(0, equals);

		value = line.substr(equals + 1);
		
		conf.set_entry(key, value);
	}
}
