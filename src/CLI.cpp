// LICENSE BY MIT 2025
#include <CLI/CLI.hpp>
#include <iostream>
#include "include/elf_parser.hpp"
#include "include/disassembler.hpp"
#include "include/editor.hpp"
#include "include/injector.hpp"

int runCLI(int argc, char** argv) {
    CLI::App app{"Reelt - Reverse Engineering ELF Linux Tool"};

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

    std::string change_comment;

    std::string inject_section = ".text";
    std::string payload_path;

    app.add_option("file", elf_path, "Path to ELF or .melf file")->required();
    app.add_flag("--headers", show_headers, "Display ELF headers");
    app.add_flag("--symbols", show_symbols, "Display symbol table");
    app.add_flag("--disasm", disasm, "Disassemble .text section");
    app.add_flag("--rom", rom_mode, "Read-only disassemble");
    app.add_flag("--em", edit_mode, "Editor mode (save .melf)");
    app.add_flag("--pm", patch_mode, "Patch mode (load .melf)");
    app.add_flag("--save", patch_save, "Save final patched ELF (use with --pm)");
    app.add_flag("--inj", inj_mode, "Inject code snippet into ELF");
    app.add_flag("--force", force_mode, "Force execution (may overwrite sections or skip safety checks)");
    app.add_option("--change", change_comment, "Comment to embed in .melf (use with --em)");

    app.add_option("--section", inject_section, "Section name to inject into (default: .text)");
    app.add_option("--payload", payload_path, "Path to binary payload file (required with --inj)");

    CLI11_PARSE(app, argc, argv);

    if (rom_mode) {
        std::cout << "[*] Running in Read-Only Disassemble Mode...\n";
        readOnlyDisassemble(elf_path);
        return 0;
    }

    if (edit_mode) {
        if (change_comment.empty()) {
            std::cerr << "[Error] --change \"your comment\" is required in --em mode\n";
            return 1;
        }
        std::cout << "[*] Running in Editor Mode...\n";
        editAndSave(elf_path, change_comment);
        std::cout << "[+] .melf file created successfully.\n";
        return 0;
    }

    if (patch_mode) {
        if (!patch_save) {
            std::cerr << "[Error] --save is required in --pm mode\n";
            return 1;
        }
        std::cout << "[*] Applying Patch from .melf...\n";
        applyPatch(elf_path);
        std::cout << "[+] ELF patched and saved.\n";
        return 0;
    }

    if (inj_mode) {
        if (payload_path.empty()) {
            std::cerr << "[Error] --payload <payload.bin> is required in --inj mode\n";
            return 1;
        }
        std::cout << "[*] Injecting code snippet into ELF...\n";
        injectCodeSnippet(elf_path, inject_section, payload_path, force_mode);
        std::cout << "[+] Injection completed successfully.\n";
        return 0;
    }

    if (!loadELF(elf_path)) {
        std::cerr << "[Error] Failed to load ELF: " << elf_path << std::endl;
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
