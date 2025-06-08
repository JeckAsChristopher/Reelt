// LICENSE BY MIT 2025

#include <iostream>
#include <fstream>
#include <vector>
#include <elfio/elfio.hpp>
#include <filesystem>
#include <chrono>
#include <iomanip>

using namespace ELFIO;

void injectCodeSnippet(const std::string& elf_path,
                       const std::string& section_name,
                       const std::string& payload_path,
                       bool force_expand = true) {
    auto start_time = std::chrono::high_resolution_clock::now();

    elfio reader;
    std::string backup_path = elf_path + ".bak";

    try {
        std::filesystem::copy(elf_path, backup_path, std::filesystem::copy_options::overwrite_existing);
        std::cout << "[+] Backup created: " << backup_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Backup creation failed: " << e.what() << std::endl;
        return;
    }

    if (!reader.load(elf_path)) {
        std::cerr << "Failed to load ELF file: " << elf_path << std::endl;
        return;
    }

    section* target_section = nullptr;
    for (int i = 0; i < reader.sections.size(); ++i) {
        section* sec = reader.sections[i];
        if (sec->get_name() == section_name) {
            target_section = sec;
            break;
        }
    }

    std::ifstream payload_file(payload_path, std::ios::binary);
    if (!payload_file) {
        std::cerr << "Failed to open payload: " << payload_path << std::endl;
        return;
    }

    std::vector<char> payload((std::istreambuf_iterator<char>(payload_file)),
                               std::istreambuf_iterator<char>());
    payload_file.close();

    if (payload.empty()) {
        std::cerr << "Payload is empty." << std::endl;
        return;
    }

    if (!target_section) {
        std::cout << "Section '" << section_name << "' not found. Creating new section..." << std::endl;

        section* new_sec = reader.sections.add(section_name);
        new_sec->set_type(SHT_PROGBITS);
        new_sec->set_flags(SHF_ALLOC | SHF_EXECINSTR);
        new_sec->set_data(payload.data(), payload.size());
        new_sec->set_addr_align(0x1000); // Good alignment for code

        if (!reader.save(elf_path)) {
            std::cerr << "Failed to save ELF with new section." << std::endl;
            return;
        }

        std::cout << "New section '" << section_name << "' added with payload (" 
                  << payload.size() << " bytes)." << std::endl;
        
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        std::cout << "Done in " << elapsed.count() << " seconds.\n";
        return;
    }

    std::cout << "Located section '" << section_name << "': offset = 0x"
              << std::hex << target_section->get_offset() << ", size = 0x"
              << target_section->get_size() << std::dec << std::endl;

    size_t original_size = target_section->get_size();

    if (payload.size() > original_size) {
        if (force_expand) {
            std::cout << "Expanding section '" << section_name << "'..." << std::endl;
            target_section->set_data(payload.data(), payload.size());
            target_section->set_size(payload.size());

            if (!reader.save(elf_path)) {
                std::cerr << "Failed to save ELF after section expansion." << std::endl;
                return;
            }

            std::cout << "Section expanded and payload injected (" 
                      << payload.size() << " bytes)." << std::endl;
        } else {
            std::cerr << "Payload too large for section and expansion disabled." << std::endl;
            return;
        }
    } else {
        std::fstream elf_file(elf_path, std::ios::in | std::ios::out | std::ios::binary);
        if (!elf_file) {
            std::cerr << "Failed to open ELF for modification." << std::endl;
            return;
        }

        elf_file.seekp(target_section->get_offset());
        elf_file.write(payload.data(), payload.size());

        if (payload.size() < original_size) {
            std::vector<char> fill(original_size - payload.size(), 0x00);
            elf_file.write(fill.data(), fill.size());
        }

        elf_file.close();
        std::cout << "Payload injected into existing section." << std::endl;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Done in " << elapsed.count() << " seconds.\n";
}
