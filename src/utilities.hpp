#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <memory>
#include <string>
#include <array>
#include <vector>
#include <string_view>

#include <sys/ioctl.h>

#include "types.hpp"

namespace Utils {
	struct DfGt {
		bool operator()(const std::unique_ptr<DesktopFile>& df1, const std::unique_ptr<DesktopFile>& df2);
	};

	void get_desktop_files(const std::array<std::string, 2>& dirs, std::vector<std::string>& file_paths);

	void split_actions(std::string_view actions, std::vector<std::string_view>& out_vec);

	void set_action_knowns(DesktopFile& action, const std::unique_ptr<DesktopFile>& poi);

	void collect_all_df(std::vector<std::unique_ptr<DesktopFile>>& as_structs, const std::array<std::string, 2>& dirs_to_search);

	void prepare_space();

	void clean_space();

	const char* remove_trailing_space(std::string& inp);

	std::vector<std::string> get_args(const std::string& exec);
}
#endif
