// LICENSE BY MIT 2025

#include <iostream>
#include <string>
#include <CLI/CLI.hpp>
#include "include/elf_parser.hpp"
#include "include/disassembler.hpp"

int main(int argc, char** argv) {
    CLI::App app{"Reelt - Reverse Engineering ELF Linux Tool"};

    std::string elf_path;
    bool show_headers = false;
    bool show_symbols = false;
    bool disasm = false;

    app.add_option("file", elf_path, "Path to ELF file")->required();
    app.add_flag("--headers", show_headers, "Show ELF headers");
    app.add_flag("--symbols", show_symbols, "Show symbol table");
    app.add_flag("--disasm", disasm, "Disassemble .text section");

    CLI11_PARSE(app, argc, argv);

    if (!loadELF(elf_path)) {
        std::cerr << "Failed to load ELF: " << elf_path << std::endl;
        return 1;
    }

    if (show_headers) {
        displayELFHeaders();
    }

    if (show_symbols) {
        displayELFSymbols();
    }

    if (disasm) {
        disassembleTextSection(elf_path);
    }

    return 0;
}
