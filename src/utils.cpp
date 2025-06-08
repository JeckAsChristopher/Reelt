// LICENSE BY MIT 2025

#include "include/utils.hpp"
#include <sstream>
#include <iomanip>
#include <bitset>
#include <elfio/elf_types.hpp>

namespace elfutil {

std::string toHex(uint64_t val, bool prefix) {
    std::ostringstream oss;
    if (prefix) oss << "0x";
    oss << std::hex << std::setw(16) << std::setfill('0') << val;
    return oss.str();
}

std::string getSectionType(uint32_t type) {
    using namespace ELFIO;

    switch (type) {
        case SHT_NULL:          return "NULL";
        case SHT_PROGBITS:      return "PROGBITS";
        case SHT_SYMTAB:        return "SYMTAB";
        case SHT_STRTAB:        return "STRTAB";
        case SHT_RELA:          return "RELA";
        case SHT_HASH:          return "HASH";
        case SHT_DYNAMIC:       return "DYNAMIC";
        case SHT_NOTE:          return "NOTE";
        case SHT_NOBITS:        return "NOBITS";
        case SHT_REL:           return "REL";
        case SHT_SHLIB:         return "SHLIB";
        case SHT_DYNSYM:        return "DYNSYM";
        case SHT_INIT_ARRAY:    return "INIT_ARRAY";
        case SHT_FINI_ARRAY:    return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY: return "PREINIT_ARRAY";
        case SHT_GROUP:         return "GROUP";
        case SHT_SYMTAB_SHNDX:  return "SYMTAB_SHNDX";
        case SHT_GNU_versym:    return "GNU_VERSYM";
        case SHT_GNU_verdef:    return "GNU_VERDEF";
        case SHT_GNU_verneed:   return "GNU_VERNEED";
        default:                return "UNKNOWN";
    }
}

std::string getSectionFlags(uint64_t flags) {
    using namespace ELFIO;

    struct FlagDef {
        uint64_t bit;
        char label;
    };

    constexpr FlagDef flagDefs[] = {
        { SHF_WRITE,      'W' },
        { SHF_ALLOC,      'A' },
        { SHF_EXECINSTR,  'X' },
        { SHF_MERGE,      'M' },
        { SHF_STRINGS,    'S' },
        { SHF_INFO_LINK,  'I' },
        { SHF_LINK_ORDER, 'L' },
        { SHF_TLS,        'T' },
        { SHF_COMPRESSED, 'C' }
    };

    std::string result;
    for (const auto& def : flagDefs) {
        if (flags & def.bit) {
            result += def.label;
        }
    }

    return result.empty() ? "-" : result;
}

}
