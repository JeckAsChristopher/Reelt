// LICENSE BY MIT 2025

#include "include/disassembler.hpp"

#include <elfio/elfio.hpp>
#include <elfio/elf_types.hpp>

#include <capstone/capstone.h>

#include <iostream>
#include <cstdio>
#include <cinttypes>

namespace elfutil {

void disassembleTextSection(const std::string& elf_path) {
    ELFIO::elfio reader;

    if (!reader.load(elf_path)) {
        std::cerr << "[Error] Failed to load ELF file: " << elf_path << '\n';
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
        std::cerr << "[Error] .text section not found in ELF\n";
        return;
    }

    const uint8_t* code     = reinterpret_cast<const uint8_t*>(text_sec->get_data());
    size_t         codeSize = text_sec->get_size();
    uint64_t       addr     = text_sec->get_address();

    csh handle;
    cs_insn* insn = nullptr;
    cs_mode mode  = (reader.get_class() == ELFIO::ELFCLASS64) ? CS_MODE_64 : CS_MODE_32;

    cs_err err = cs_open(CS_ARCH_X86, mode, &handle);
    if (err != CS_ERR_OK) {
        std::cerr << "[Capstone Error] " << cs_strerror(err) << '\n';
        return;
    }

    cs_option(handle, CS_OPT_DETAIL, CS_OPT_OFF);

    size_t count = cs_disasm(handle, code, codeSize, addr, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; ++i) {
            std::printf("0x%" PRIx64 ": %-10s %s\n", 
                        insn[i].address, 
                        insn[i].mnemonic, 
                        insn[i].op_str);
        }
        cs_free(insn, count);
    } else {
        std::cerr << "[Error] Failed to disassemble code in .text section\n";
    }

    cs_close(&handle);
}

}
