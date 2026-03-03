#include <algorithm>

#include "constants.hpp"
#include "finder.hpp"

int main_finder(std::size_t inplen, std::string& substr, int current_modifier, const std::string& current_lower) {
	int match_score = 0;
	std::size_t current_offset = 0;
	std::size_t next_offset = 0;
	bool is_word_begin = false;
	bool is_word_end = false;
	for (std::size_t j = 0; j < inplen; ++j) {
		for (std::size_t i = substr.length(); i > 0; --i) {
			next_offset = current_lower.find(substr.substr(0,i), current_offset);
			if (next_offset != std::string::npos) {
				if (next_offset == 0  ||
						!std::isalpha(current_lower[next_offset - 1])) is_word_begin = true;
				if (next_offset + i == current_lower.length() ||
						!std::isalpha(current_lower[next_offset + i])) is_word_end = true;

				match_score += ((is_word_begin*word_begin_modifier + is_word_end*word_end_modifier + 
							     current_modifier)*i - next_offset);
				current_offset = next_offset + i;
				substr = substr.substr(i);
				goto full_continue;
			}
		}
		if (current_modifier == in_title_modifier) return match_score;
		if (current_modifier == in_comment_modifier) return match_score;
		if (current_modifier == in_keywords_modifier) return match_score;
		return 0;

full_continue:
		if (substr.length() == 0) return match_score;
		is_word_begin = false;
		is_word_end = false;
	}


	return match_score;
}

void fuzzyfind(std::unique_ptr<DesktopFile>& df, const std::string& inp) {
	int match_score = 0;
	std::string substr { inp };
	std::string lower { df->Name };

	std::for_each(lower.begin(), lower.end(), [](char& c) { c = std::tolower(c); });

	for (int index = 0; index < 4; ++index) {
		switch (index) {
			case 0:
				match_score += main_finder(inp.length(), substr, in_title_modifier, lower);
				break;
			case 1:
				lower = df->Comment;
				std::for_each(lower.begin(), lower.end(), [](char& c) { c = std::tolower(c); });
				match_score += main_finder(inp.length(), substr, in_comment_modifier, lower);
				break;
			case 2:
				lower = df->Keywords;
				std::for_each(lower.begin(), lower.end(), [](char& c) { c = std::tolower(c); });
				match_score += main_finder(inp.length(), substr, in_keywords_modifier, lower);
				break;
			case 3:
				lower = df->Categories;
				std::for_each(lower.begin(), lower.end(), [](char& c) { c = std::tolower(c); });
				match_score += main_finder(inp.length(), substr, in_categories_modifier, lower);
				break;
		}
		if (substr.length() == 0) break;
	}
	if (substr.length() > 0) match_score = 0;

	df->match_score = match_score;
}
