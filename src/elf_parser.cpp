#include "include/elf_parser.hpp"
#include <elfio/elfio.hpp>
#include <iostream>
#include <iomanip>

static ELFIO::elfio reader;

bool loadELF(const std::string& path) {
    return reader.load(path);
}

void displayELFHeaders() {
    std::cout << "ELF Header:\n";
    std::cout << "  Class:      " << (reader.get_class() == ELFIO::ELFCLASS64 ? "ELF64" : "ELF32") << "\n";
    std::cout << "  Endianness: " << (reader.get_encoding() == ELFIO::ELFDATA2LSB ? "Little Endian" : "Big Endian") << "\n";
    std::cout << "  Entry:      0x" << std::hex << reader.get_entry() << std::dec << "\n";

    std::cout << "\nSections:\n";
    for (const auto& sec : reader.sections) {
        std::cout << "  [" << std::setw(2) << sec->get_index() << "] "
                  << std::setw(20) << std::left << sec->get_name()
                  << " size: " << sec->get_size()
                  << " addr: 0x" << std::hex << sec->get_address() << std::dec << "\n";
    }
}

void displayELFSymbols() {
    // Need non-const pointer for symbol_section_accessor
    ELFIO::section* sym_sec = reader.sections[".symtab"];
    if (!sym_sec) {
        std::cerr << "No symbol table found (.symtab)\n";
        return;
    }

    ELFIO::symbol_section_accessor symbols(reader, sym_sec);
    std::cout << "\nSymbols:\n";

    for (unsigned int i = 0; i < symbols.get_symbols_num(); ++i) {
        std::string name;
        ELFIO::Elf64_Addr value;
        ELFIO::Elf_Xword size;
        unsigned char bind, type, other;
        ELFIO::Elf_Half shndx;

        symbols.get_symbol(i, name, value, size, bind, type, shndx, other);

        std::cout << "  " << std::setw(25) << std::left << name
                  << " addr: 0x" << std::hex << value
                  << " size: " << std::dec << size << "\n";
    }
}
