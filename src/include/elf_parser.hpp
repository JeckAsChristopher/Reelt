#ifndef REELT_ELF_PARSER_HPP
#define REELT_ELF_PARSER_HPP

#include <string>

namespace elfutil {

bool loadELF(const std::string& path);
void displayELFHeaders();
void displayELFSymbols();

}

#endif // REELT_ELF_PARSER_HPP
