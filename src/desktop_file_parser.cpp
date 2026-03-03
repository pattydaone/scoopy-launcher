#include <cstddef>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <iomanip>
#include "desktop_file_parser.hpp"
#include "types.hpp"
#include "utilities.hpp"

namespace Prsr {
	std::unique_ptr<DesktopFile> get_desktop_struct_pointer(const std::string& path) {
		DesktopFile parsed;
		std::ifstream desktop_file { path };
		std::string line;
		std::string key;
		std::string value;
		std::size_t equals;

		while (std::getline(desktop_file, line)) {
			equals = line.find_first_of("=");
			if (equals == std::string::npos) { 
				if (line.find("Desktop Action") != std::string::npos) { break; }
			}

			key = line.substr(0, equals);

			value = line.substr(equals + 1);
			parsed.set_entry(key, value);
		}

		return std::make_unique<DesktopFile> ( parsed );
	}

	void append_actions(const std::string& path, std::vector<std::unique_ptr<DesktopFile>>& desktop_structs) {
		const std::size_t dfoi = desktop_structs.size() - 1;
		std::vector<std::string_view> actions;
		Utils::split_actions(desktop_structs[dfoi]->Actions, actions);
		std::ifstream desktop_file { path };
		std::string line;
		std::size_t equals;
		std::string key;
		std::string value;

		for (const auto action : actions) {
			bool action_found = false;
			std::stringstream to_find;
			DesktopFile df_action;
			Utils::set_action_knowns(df_action, desktop_structs[dfoi]);

			to_find << "[Desktop Action " << action << "]";
			while (std::getline(desktop_file, line)) {
				if (!action_found && line.find(to_find.str()) != std::string::npos) {
					action_found = true;
					continue;
				}

				if (!action_found) continue;

				equals = line.find_first_of("=");
				key = line.substr(0, equals);
				value = line.substr(equals + 1);

				if (key == "Name") df_action.Name = value;
				else if (key == "Exec") {
					df_action.Exec = value;
					break;
				}
			}
			desktop_structs.push_back(std::make_unique<DesktopFile> (df_action));
		}
	}
}
