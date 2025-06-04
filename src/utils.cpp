// LICENSE BY MIT 2025

#include "include/utils.hpp"
#include <sstream>
#include <iomanip>
#include <elfio/elf_types.hpp>

std::string toHex(uint64_t val, bool prefix) {
    std::ostringstream oss;
    if (prefix)
        oss << "0x";
    oss << std::hex << std::setw(8) << std::setfill('0') << val;
    return oss.str();
}

std::string getSectionType(uint32_t type) {
    using namespace ELFIO;

    switch (type) {
        case SHT_NULL:     return "NULL";
        case SHT_PROGBITS: return "PROGBITS";
        case SHT_SYMTAB:   return "SYMTAB";
        case SHT_STRTAB:   return "STRTAB";
        case SHT_RELA:     return "RELA";
        case SHT_HASH:     return "HASH";
        case SHT_DYNAMIC:  return "DYNAMIC";
        case SHT_NOTE:     return "NOTE";
        case SHT_NOBITS:   return "NOBITS";
        case SHT_REL:      return "REL";
        case SHT_SHLIB:    return "SHLIB";
        case SHT_DYNSYM:   return "DYNSYM";
        default:           return "UNKNOWN";
    }
}
