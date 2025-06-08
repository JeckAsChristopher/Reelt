#ifndef REELT_DISASSEMBLER_HPP
#define REELT_DISASSEMBLER_HPP

#include <string>

namespace elfutil {

void disassembleTextSection(const std::string& elf_path);

}

#endif // REELT_DISASSEMBLER_HPP
