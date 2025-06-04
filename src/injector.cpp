// LICENSE BY MIT 2025

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <elfio/elfio.hpp>
#include "include/injector.hpp"

void injectCodeSnippet(const std::string& elf_path,
                       const std::string& section_name,
                       const std::string& payload_path) {
    ELFIO::elfio reader;

    if (!reader.load(elf_path)) {
        std::cerr << "[-] Failed to load ELF file: " << elf_path << std::endl;
        return;
    }

    const ELFIO::section* sec = reader.sections[section_name];
    if (!sec) {
        std::cerr << "[-] Section " << section_name << " not found in ELF." << std::endl;
        return;
    }

    std::cout << "[*] " << section_name << " found: offset = 0x"
              << std::hex << sec->get_offset()
              << ", size = 0x" << sec->get_size() << std::dec << std::endl;

    std::ifstream payload_file(payload_path, std::ios::binary);
    if (!payload_file) {
        std::cerr << "[-] Failed to open payload file: " << payload_path << std::endl;
        return;
    }

    std::vector<char> payload((std::istreambuf_iterator<char>(payload_file)),
                               std::istreambuf_iterator<char>());

    if (payload.size() > sec->get_size()) {
        std::cerr << "[-] Payload size (" << payload.size() << ") exceeds section size (" << sec->get_size() << ")." << std::endl;
        return;
    }

    payload_file.close();

    std::fstream elf_file(elf_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!elf_file) {
        std::cerr << "[-] Failed to open ELF file for writing: " << elf_path << std::endl;
        return;
    }

    elf_file.seekp(sec->get_offset());
    if (!elf_file) {
        std::cerr << "[-] Failed to seek to section offset." << std::endl;
        elf_file.close();
        return;
    }

    elf_file.write(payload.data(), payload.size());
    if (!elf_file) {
        std::cerr << "[-] Failed to write payload to section." << std::endl;
        elf_file.close();
        return;
    }

    elf_file.close();

    std::cout << "[+] Injection completed successfully." << std::endl;
}
