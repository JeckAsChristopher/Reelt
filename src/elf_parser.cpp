// LICENSE BY MIT 2025

#include "include/elf_parser.hpp"
#include <elfio/elfio.hpp>
#include <iostream>
#include <iomanip>
#include <string>

namespace elfutil {

static ELFIO::elfio reader;

// Load ELF file
bool loadELF(const std::string& path) {
    if (!reader.load(path)) {
        std::cerr << "[Error] Failed to load ELF: " << path << '\n';
        return false;
    }
    return true;
}

void displayELFHeaders() {
    std::cout << "== ELF Header ==\n";
    std::cout << "  Class:      " 
              << (reader.get_class() == ELFIO::ELFCLASS64 ? "ELF64" : "ELF32") << '\n';

    std::cout << "  Endianness: "
              << (reader.get_encoding() == ELFIO::ELFDATA2LSB ? "Little Endian" : "Big Endian") << '\n';

    std::cout << "  Entry:      "
              << std::showbase << std::hex << reader.get_entry() << std::dec << '\n';

    std::cout << "\n== Sections ==\n";
    std::cout << std::left << std::setw(6) << "[#]"
              << std::setw(22) << "Name"
              << std::setw(12) << "Size"
              << "Address\n";

    for (const auto& sec : reader.sections) {
        std::cout << "  [" << std::setw(2) << sec->get_index() << "] "
                  << std::setw(22) << std::left << sec->get_name()
                  << std::setw(12) << sec->get_size()
                  << std::showbase << std::hex << sec->get_address() << std::dec << '\n';
    }
}

void displayELFSymbols() {
    ELFIO::section* sym_sec = reader.sections[".symtab"];
    if (!sym_sec) {
        std::cerr << "[Warning] No symbol table found (.symtab)\n";
        return;
    }

    ELFIO::symbol_section_accessor symbols(reader, sym_sec);
    const auto total = symbols.get_symbols_num();

    if (total == 0) {
        std::cout << "[Info] Symbol table is empty\n";
        return;
    }

    std::cout << "\n== Symbols ==\n";
    std::cout << std::left << std::setw(30) << "Name"
              << std::setw(18) << "Address"
              << "Size\n";

    for (unsigned int i = 0; i < total; ++i) {
        std::string name;
        ELFIO::Elf64_Addr value;
        ELFIO::Elf_Xword size;
        unsigned char bind, type, other;
        ELFIO::Elf_Half shndx;

        if (symbols.get_symbol(i, name, value, size, bind, type, shndx, other)) {
            std::cout << "  " << std::setw(30) << std::left << name
                      << std::setw(18) << std::showbase << std::hex << value
                      << std::dec << size << '\n';
        }
    }
}

}
