# Reelt - Reverse Engineering Elf Linux Tool

## What is Reelt?
**Reelt** is where you can inspect **headers**, **symbols**, **disassemble**, and combine the options
It uses 3 third-party tools such as **ELFIO, Capstone, and CLI**
> Im working on the modification tool where you can modify the variables now

---

## Version
**0.1-prerelease** - the features is only inspect you can't modify the assembly code.

---

## Usage

- **./xpriv --headers examplelf** - See only headers

- **./xpriv --symbols examplelf** - See only symbols from machine code(assembly)

- **./xpriv --disasm examplelf** - See only disassembly code(doesn't do much from now.)

- **./reelt --headers --symbols --disasm examplelf** - Can see all such as **--headers, --symbols --disasm**.

## Clone Repository

```bash
git clone https://github.com/JeckAsChristopher/Reelt

// Or Just Download Directly from the website.
```

## This has built-in libraries so you don't need to install any libraries.
