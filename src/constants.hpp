#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <array>
#include <cstdint>
#include <cstdlib>

const std::string home_dir { std::getenv("HOME") };
const std::array<std::string, 2> dirs_to_search {
    "/usr/share/applications", home_dir + "/.local/share/applications"
};

constexpr int word_begin_modifier = 10;
constexpr int word_end_modifier = 5;
constexpr int in_title_modifier = 12;
constexpr int in_comment_modifier = 7;
constexpr int in_keywords_modifier = 5;
constexpr int in_categories_modifier = 3;

constexpr std::uint64_t update_freq = 8;

#endif
