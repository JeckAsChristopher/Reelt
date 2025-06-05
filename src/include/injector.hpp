#ifndef INJECTOR_HPP
#define INJECTOR_HPP

#include <string>

void injectCodeSnippet(const std::string& elf_path,
                       const std::string& section_name,
                       const std::string& payload_path,
bool force);

#endif
