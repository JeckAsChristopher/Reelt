#include <iostream>
#include <fstream>
#include <vector>
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

    ELFIO::section* target_section = nullptr;
    for (int i = 0; i < reader.sections.size(); ++i) {
        ELFIO::section* sec = reader.sections[i];
        if (sec->get_name() == section_name) {
            target_section = sec;
            break;
        }
    }

    if (!target_section) {
        std::cerr << "[-] Section '" << section_name << "' not found in ELF." << std::endl;
        return;
    }

    std::cout << "[*] Located section '" << section_name << "': "
              << "offset = 0x" << std::hex << target_section->get_offset()
              << ", size = 0x" << target_section->get_size() << std::dec << std::endl;

    std::ifstream payload_file(payload_path, std::ios::binary);
    if (!payload_file) {
        std::cerr << "[-] Failed to open payload file: " << payload_path << std::endl;
        return;
    }

    std::vector<char> payload((std::istreambuf_iterator<char>(payload_file)),
                              std::istreambuf_iterator<char>());
    payload_file.close();

    if (payload.empty()) {
        std::cerr << "[-] Payload is empty." << std::endl;
        return;
    }

    size_t section_size = target_section->get_size();

    if (payload.size() > section_size) {
        std::cout << "[*] Payload is larger than current section. Expanding section..." << std::endl;

        target_section->set_data(payload.data(), payload.size());
        target_section->set_size(payload.size());

        if (!reader.save(elf_path)) {
            std::cerr << "[-] Failed to save updated ELF after section expansion." << std::endl;
            return;
        }

        std::cout << "[+] Section '" << section_name << "' expanded and payload injected." << std::endl;
    } else {
        std::fstream elf_file(elf_path, std::ios::in | std::ios::out | std::ios::binary);
        if (!elf_file) {
            std::cerr << "[-] Failed to open ELF file for writing: " << elf_path << std::endl;
            return;
        }

        elf_file.seekp(target_section->get_offset());
        if (!elf_file) {
            std::cerr << "[-] Failed to seek to section offset." << std::endl;
            elf_file.close();
            return;
        }

        elf_file.write(payload.data(), payload.size());
        if (!elf_file) {
            std::cerr << "[-] Failed to write payload into ELF section." << std::endl;
            elf_file.close();
            return;
        }

        if (payload.size() < section_size) {
            size_t remaining = section_size - payload.size();
            std::vector<char> zeros(remaining, '\0');
            elf_file.write(zeros.data(), remaining);
            if (!elf_file) {
                std::cerr << "[-] Failed to zero out remaining section bytes." << std::endl;
                elf_file.close();
                return;
            }
        }

        elf_file.close();
        std::cout << "[+] Payload injected into section '" << section_name << "' without expansion." << std::endl;
    }
}
