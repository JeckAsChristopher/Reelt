#ifndef REELT_EDITOR_HPP
#define REELT_EDITOR_HPP

#include <string>

void readOnlyDisassemble(const std::string& elf_path);
void editAndSave(const std::string& elf_path, const std::string& comment);
void applyPatch(const std::string& melf_path);

#endif // REELT_EDITOR_HPP
