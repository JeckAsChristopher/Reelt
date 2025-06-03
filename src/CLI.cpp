#include <CLI/CLI.hpp>
#include "include/elf_parser.hpp"
#include "include/disassembler.hpp"
#include <iostream>

// Main CLI function now returns int
int runCLI(int argc, char** argv) {
    CLI::App app{"Reelt - Reverse Engineering ELF Linux Tool"};

    std::string elf_path;
    bool show_headers = false;
    bool show_symbols = false;
    bool disasm = false;

    // Set up CLI options and flags
    app.add_option("file", elf_path, "Path to ELF file")->required();
    app.add_flag("--headers", show_headers, "Display ELF headers");
    app.add_flag("--symbols", show_symbols, "Display symbol table");
    app.add_flag("--disasm", disasm, "Disassemble .text section");

    // CLI11_PARSE expands to return, so runCLI must return int
    CLI11_PARSE(app, argc, argv);

    // Load ELF file
    if (!loadELF(elf_path)) {
        std::cerr << "[Error] Failed to load ELF: " << elf_path << std::endl;
        return 1;
    }

    // Display headers if requested
    if (show_headers) {
        displayELFHeaders();
    }

    // Display symbols if requested
    if (show_symbols) {
        displayELFSymbols();
    }

    // Disassemble .text section if requested
    if (disasm) {
        disassembleTextSection(elf_path);
    }

    return 0;
}
