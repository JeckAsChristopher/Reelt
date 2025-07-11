// LICENSE BY MIT 2025
#include <iostream>
#include <string>
#include <unistd.h>

#include <CLI/CLI.hpp>
#include "include/elf_parser.hpp"
#include "include/disassembler.hpp"
#include "include/editor.hpp"
#include "include/injector.hpp"
#include "include/version.hpp"

// ANSI color macros
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

int main(int argc, char** argv) {
    if (geteuid() == 0) {
        std::cout << BOLD RED "You're on sudo mode. Please use the software at your own risk.\n" RESET;
    }

    CLI::App app{CYAN "Reelt - Reverse Engineering ELF Linux Tool" RESET};

    std::string elf_path;
    bool show_headers = false;
    bool show_symbols = false;
    bool disasm = false;
    bool rom_mode = false;
    bool edit_mode = false;
    bool patch_mode = false;
    bool patch_save = false;
    bool inj_mode = false;
    bool force_mode = false;
    bool show_version = false;

    std::string change_comment;
    std::string section_name;
    std::string payload_path;

    app.add_flag("--version", show_version, "Show version and exit");

    app.add_option("file", elf_path, "Path to ELF or .melf file");
    app.add_flag("--headers", show_headers, "Show ELF headers");
    app.add_flag("--symbols", show_symbols, "Show symbol table");
    app.add_flag("--disasm", disasm, "Disassemble .text section");
    app.add_flag("--rom", rom_mode, "Read-only disassemble mode");
    app.add_flag("--em", edit_mode, "Editor mode (generate .melf)");
    app.add_flag("--pm", patch_mode, "Patch mode (apply .melf)");
    app.add_flag("--save", patch_save, "Save patched ELF (use with --pm)");
    app.add_flag("--inj", inj_mode, "Inject code into ELF (requires --section and --payload)");
    app.add_flag("--force", force_mode, "Force override any warnings or prompts");

    app.add_option("--change", change_comment, "Comment to embed in .melf (required with --em)");
    app.add_option("--section", section_name, "Section name to inject into (e.g. .rodata)");
    app.add_option("--payload", payload_path, "Path to payload binary file for injection");

    CLI11_PARSE(app, argc, argv);

    if (show_version) {
        if (!verifyVersionIntegrity()) {
            std::cerr << RED "WARNING: Version integrity check failed. Binary may be tampered.\n" RESET;
        } else {
            std::cout << CYAN << "Reelt Version: " << getSecureVersion() << RESET << "\n";
        }
        return 0;
    }

    if (elf_path.empty()) {
        std::cerr << RED "Error: No ELF file specified. Use --help for usage.\n" RESET;
        return 1;
    }

    if (rom_mode) {
        std::cout << YELLOW "Read-only disassembly mode enabled.\n" RESET;
        readOnlyDisassemble(elf_path);
        return 0;
    }

    if (edit_mode) {
        if (change_comment.empty() && !force_mode) {
            std::cerr << RED "Error: --change is required in --em mode. Use --force to override.\n" RESET;
            return 1;
        }
        std::cout << YELLOW "Editor mode enabled.\n" RESET;
        editAndSave(elf_path, change_comment.empty() ? "[NO COMMENT - FORCED]" : change_comment);
        std::cout << GREEN ".melf file generated successfully.\n" RESET;
        return 0;
    }

    if (patch_mode) {
        if (!patch_save && !force_mode) {
            std::cerr << RED "Error: --save is required in --pm mode. Use --force to override.\n" RESET;
            return 1;
        }
        std::cout << YELLOW "Applying patch from .melf...\n" RESET;
        applyPatch(elf_path);
        std::cout << GREEN "ELF patched and saved successfully.\n" RESET;
        return 0;
    }

    if (inj_mode) {
        if ((section_name.empty() || payload_path.empty()) && !force_mode) {
            std::cerr << RED "Error: --section and --payload are required in --inj mode. Use --force to override.\n" RESET;
            return 1;
        }
        std::cout << YELLOW "Injection mode enabled. Modifying ELF...\n" RESET;
        injectCodeSnippet(
            elf_path,
            section_name.empty() ? ".force" : section_name,
            payload_path.empty() ? "/dev/null" : payload_path,
            false
        );
        std::cout << GREEN "Code injection completed.\n" RESET;
        return 0;
    }

    if (!elfutil::loadELF(elf_path)) {
        std::cerr << RED "Failed to load ELF file: " << elf_path << "\n" RESET;
        return 1;
    }

    if (show_headers) {
        std::cout << CYAN "Displaying ELF headers...\n" RESET;
        elfutil::displayELFHeaders();
    }

    if (show_symbols) {
        std::cout << CYAN "Displaying ELF symbol table...\n" RESET;
        elfutil::displayELFSymbols();
    }

    if (disasm) {
        std::cout << CYAN "Disassembling .text section...\n" RESET;
        elfutil::disassembleTextSection(elf_path);
    }

    std::cout << GREEN "Operation completed.\n" RESET;
    return 0;
}
