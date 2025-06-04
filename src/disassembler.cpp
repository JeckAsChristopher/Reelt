// LICENSE BY MIT 2025

#include "include/disassembler.hpp"
#include "elfio/elfio.hpp"
#include "elfio/elf_types.hpp"
#include <capstone/capstone.h>

#include <iostream>
#include <cstdio>
#include <cinttypes>

void disassembleTextSection(const std::string& elf_path) {
    ELFIO::elfio reader;
    if (!reader.load(elf_path)) {
        std::cerr << "Failed to load ELF file\n";
        return;
    }

    const ELFIO::section* text_sec = nullptr;
    for (const auto& sec : reader.sections) {
        if (sec->get_name() == ".text") {
            text_sec = sec.get();
            break;
        }
    }

    if (!text_sec) {
        std::cerr << "No .text section found\n";
        return;
    }

    const uint8_t* code = reinterpret_cast<const uint8_t*>(text_sec->get_data());
    size_t code_size = text_sec->get_size();
    uint64_t addr = text_sec->get_address();

    csh handle;
    cs_insn* insn = nullptr;
    size_t count = 0;

    cs_mode mode = (reader.get_class() == ELFIO::ELFCLASS64) ? CS_MODE_64 : CS_MODE_32;

    if (cs_open(CS_ARCH_X86, mode, &handle) != CS_ERR_OK) {
        std::cerr << "Failed to initialize Capstone\n";
        return;
    }

    count = cs_disasm(handle, code, code_size, addr, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            std::printf("0x%" PRIx64 ": %-10s %s\n", insn[i].address, insn[i].mnemonic, insn[i].op_str);
        }
        cs_free(insn, count);
    } else {
        std::cerr << "Disassembly failed\n";
    }

    cs_close(&handle);
}
