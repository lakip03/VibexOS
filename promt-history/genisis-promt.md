You are tasked with writing a minimal custom bootloader in x86_64 Assembly, which:

    Does not use any external libraries or helper tools (e.g., no GRUB, no BIOS interrupts beyond standard).

    Must compile into a bootable .img file that can be run directly using QEMU in legacy BIOS mode (MBR, not UEFI).

    When loaded, it should display the message:
    "I'm sorry Dave, I am afraid I can't let you do that"

    The bootloader must be architected to allow for future expansion (e.g., space left in sectors, modular structure).

📁 Project structure

    /src/ → All source code (Assembly).

    /dist/ → Compiled output (.img file).

    /Makefile → Build logic.

✅ Bootloader requirements

    Written in pure Assembly (NASM syntax preferred).

    Loads at 0x7C00 (standard MBR bootloader entry point).

    Compiles into a flat binary (512 bytes total, with valid boot signature 0xAA55 at the end).

    Message printed using BIOS INT 0x10.

📦 Output

    src/bootloader.asm → contains the bootloader.

    dist/os.img → bootable image file.

    Makefile → builds everything from root project directory.

🧾 Deliverables (LLM Output Expected):

    bootloader.asm file content (NASM syntax).

    Makefile with instructions to build the .img.

    Explanations of:

        Key sections of Assembly (where it loads, how message is printed).

        How it leaves room for future code (e.g., filling unused sector space).