# Reelt - Reverse Engineering Elf Linux Tool

[![License](https://img.shields.io/github/license/JeckAsChristopher/Reelt?style=flat-square)](https://github.com/JeckAsChristopher/Reelt/blob/main/LICENSE)
![Pre-release](https://img.shields.io/badge/status-pre--release-yellow)

## What is Reelt?
**Reelt** is where you can inspect **headers**, **symbols**, **disassemble**, and combine the options
It uses 3 third-party tools such as **ELFIO, Capstone, and CLI**
> Im working on the modification tool where you can modify the variables now

---

## Version
**0.1-prerelease** - the features is only inspect you can't modify the assembly code.
**0.2-prerelease** - Better compiling and libraries installation since Github make the libraries as a subdirectory.

---

## Usage

- **./xpriv --headers examplelf** - See only headers

- **./xpriv --symbols examplelf** - See only symbols from machine code(assembly)

- **./xpriv --disasm examplelf** - See only disassembly code(doesn't do much from now.)

- **./reelt --headers --symbols --disasm examplelf** - Can see all such as **--headers, --symbols --disasm**.

---

## Clone Repository

```bash
git clone https://github.com/JeckAsChristopher/Reelt

# Or just Download it directly from the Website.
```

---

# Installation
```bash
make
```
**It will prompt you to reinstall the libraries type `y` because when you clone the repository libraries is empty.**
**You need at least 500-700 mb free storage to install the libraries**
> Reinstall libraries every 2-3 days guaranteed there's an update.

---

# Notice!
### This project is still under active development!
