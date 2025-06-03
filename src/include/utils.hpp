#ifndef REELT_UTILS_HPP
#define REELT_UTILS_HPP

#include <string>

std::string toHex(uint64_t val, bool prefix = true);
std::string getSectionType(uint32_t type);

#endif // REELT_UTILS_HPP
