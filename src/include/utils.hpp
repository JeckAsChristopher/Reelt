#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cstdint>

std::string toHex(uint64_t val, bool prefix = false);
std::string getSectionType(uint32_t type);
std::string getSectionFlags(uint64_t flags);

#endif // UTILS_HPP
