#ifndef FINDER_HPP
#define FINDER_HPP

#include <memory>
#include <string>

#include "types.hpp"

void fuzzyfind(std::unique_ptr<DesktopFile>& df, const std::string& inp);

#endif
