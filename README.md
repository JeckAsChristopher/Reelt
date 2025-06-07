# Reelt - Reverse Engineering ELF Linux Tool

[![License](https://img.shields.io/github/license/JeckAsChristopher/Reelt?style=flat-square)](https://github.com/JeckAsChristopher/Reelt/blob/main/LICENSE)
[![Issues](https://img.shields.io/github/issues/JeckAsChristopher/Reelt?style=flat-square)](https://github.com/JeckAsChristopher/Reelt/issues)


## What is Reelt?

**Reelt** is a pretty strong command line interface using which the reverse engineering of ELF (Executable and Linkable Format) files done on a Linux machine can be performed.
It lets you view **headers**, **symbols**, and **disassembled code** with advanced **editing and patching** features still in the works.
Reelt is developed upon three foundational libraries:
- [**ELFIO**](https://github.com/serge1/ELFIO)
- [**Capstone**](https://www.capstone-engine.org/)
- **Custom CLI Parser**

> Variable and memory modification functionality are **under development**.

---

## Versions
**Current Version: v0.6**

| Version         | Features                                                                 |
|-----------------|--------------------------------------------------------------------------|
| **0.1-prerelease** | Minimal inspection only: headers, symbols, disassembly                   |
| **0.2-prerelease** | Better build system, library handling in subdirectories                |
| **0.3**         | experimental patching and editing capabilities                      |
| **0.4**         | Optimize configuring and building. **Added new commamd --payload and --section**              |
| **0.5**         | New command has been released. the command is `--force`. **optimize and enhance proper build, suppports on many linux distros.**      |
| **0.6**         | Fix compilation error(sorry for the incovinience that you all has an error during compilation.) Instead of exporting it on HOME i've decided to make it better.   |


---

## Features & Usage

```bash
./xpriv --headers <file> 
```

```bash
./xpriv --symbols <file>
./xpriv --disasm <file>
./reelt --headers --symbols --disasm <file>
```

### Modes (Experimental)

```bash
./reelt --rom <file>
./reelt --em <file>
```
```bash
./reelt --pm <file>
```

### Pretty useful commands
```bash
./reelt --payload - verifying and checking checksum on the elf and modifying checksum to not damage or corrupt the ELF

./reelt --inj - injects payload using raw text .bin

./reelt --section - use this if you have a target in elf files such as .text .rodata etc.

```

> These are **under development** modes and might not function as intended.

---

## Installation

```bash
git clone https://github.com/JeckAsChristopher/Reelt
cd Reelt
make
```

> In the process of building, the tool may ask to install/update libraries.
> Select `y` to continue. At least **500 to 700 MB** free space is needed.

**Note:** Library updates are common. Reinstall every **2 to 3 days** to remain updated.

---

## Disclaimer

This project is in **active development**.
Use at your own risk, and test on non-critical binaries.

---

## Contributions & Feedback

Open issues, ask for features, or contribute through pull requests, as you wish.
Together we can make **Reelt** the definitive tool for ELF reverse engineering on Linux!

---

## Credits

**Thanks to ELFIO, capstone, and CLI11** For helping me build this kind of tool!
