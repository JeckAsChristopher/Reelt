// LICENSE BY MIT 2025

#include "include/editor.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <capstone/capstone.h>
#include <elfio/elfio.hpp>

using namespace ELFIO;

void disassembleSection(const std::string& elf_path) {
    elfio reader;
    if (!reader.load(elf_path)) {
        std::cerr << "Failed to load ELF: " << elf_path << "\n";
        return;
    }

    section* text = reader.sections[".text"];
    if (!text) {
        std::cerr << "No .text section found\n";
        return;
    }

    const uint8_t* code = (const uint8_t*)text->get_data();
    size_t size = text->get_size();
    uint64_t address = text->get_address();

    csh handle;
    cs_insn* insn;
    size_t count;

    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
        std::cerr << "Failed to initialize Capstone\n";
        return;
    }

    count = cs_disasm(handle, code, size, address, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            printf("0x%" PRIx64 ": %-8s %s\n", insn[i].address, insn[i].mnemonic, insn[i].op_str);
        }
        cs_free(insn, count);
    } else {
        std::cerr << "Disassembly failed\n";
    }

    cs_close(&handle);
}

void readOnlyDisassemble(const std::string& elf_path) {
    std::cout << "[Reelt ROM Mode] Inspecting ELF: " << elf_path << "\n";
    disassembleSection(elf_path);
}

void editAndInject(const std::string& elf_path, const std::string& section_name, const std::string& new_data) {
    elfio reader;
    if (!reader.load(elf_path)) {
        std::cerr << "Failed to load ELF: " << elf_path << "\n";
        return;
    }

    section* sec = reader.sections[section_name];
    if (!sec) {
        std::cerr << "Section not found: " << section_name << "\n";
        return;
    }

    // Replace section data
    char* buf = new char[new_data.size()];
    memcpy(buf, new_data.data(), new_data.size());
    sec->set_data(buf, new_data.size());

    std::string melf_path = elf_path + ".melf";
    if (!reader.save(melf_path)) {
        std::cerr << "Failed to save modified ELF as: " << melf_path << "\n";
    } else {
        std::cout << "[Reelt Inject Mode] Section '" << section_name << "' modified and saved as: " << melf_path << "\n";
    }

    delete[] buf;
}

void editAndSave(const std::string& elf_path, const std::string& comment) {
    std::ifstream input(elf_path, std::ios::binary);
    if (!input) {
        std::cerr << "Failed to open ELF: " << elf_path << "\n";
        return;
    }

    std::ofstream output(elf_path + ".melf", std::ios::binary);
    if (!output) {
        std::cerr << "Failed to write to: " << elf_path << ".melf\n";
        return;
    }

    output << input.rdbuf();
    output << "\n// REELT-COMMENT: " << comment << "\n";

    std::cout << "[Reelt Editor Mode] Saved as: " << elf_path << ".melf\n";
}

void applyPatch(const std::string& melf_path) {
    std::ifstream input(melf_path, std::ios::binary);
    if (!input) {
        std::cerr << "Failed to open .melf file: " << melf_path << "\n";
        return;
    }

    // Read full content and trim at comment marker if found
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), {});
    auto it = std::search(buffer.begin(), buffer.end(), 
                          std::begin("// REELT-COMMENT"), std::end("// REELT-COMMENT"));

    std::string output_path = melf_path.substr(0, melf_path.find(".melf"));  // No extension
    std::ofstream output(output_path, std::ios::binary);
    if (!output) {
        std::cerr << "Failed to write patched ELF: " << output_path << "\n";
        return;
    }

    size_t length = (it != buffer.end()) ? std::distance(buffer.begin(), it) : buffer.size();
    output.write(buffer.data(), length);

    std::cout << "[Reelt Patch Mode] Final ELF saved as: " << output_path << "\n";

    // Dump .text
    elfio reader;
    if (reader.load(output_path)) {
        section* text = reader.sections[".text"];
        if (text) {
            std::ofstream rom_dump("romdump.bin", std::ios::binary);
            if (rom_dump) {
                rom_dump.write(text->get_data(), text->get_size());
                std::cout << "[ROM Dump] .text section extracted to romdump.bin\n";
            } else {
                std::cerr << "Failed to create romdump.bin\n";
            }
        } else {
            std::cerr << "No .text section found in patched ELF\n";
        }
    } else {
        std::cerr << "Failed to reload patched ELF for dump\n";
    }
}
