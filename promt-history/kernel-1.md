You are developing a custom monolithic kernel named VibexOS. Write code that satisfies the following architecture and constraints:
📜 Project Requirements:

    Architecture: Targeting x86_64, running in real mode initially, then transitioning to 32-bit protected mode.

    Bootloader: Written in NASM, already exists in src/bootloader.asm. It loads and jumps to the kernel binary.

    Kernel:

        Implemented in C (compiled with gcc -m32).

        Entry point (kernel_main) written in C and compiled to a flat binary.

        Prints a hardcoded message:
        "Welcome to VibexOS"

        Linked properly using a linker script.

    Build System:

        Extend the Makefile from previous step to:

            Compile kernel.c (placed in src/) into a flat binary.

            Link it with a linker script into an ELF or flat kernel image.

            Concatenate the bootloader and kernel to form a bootable .img file.

    Output layout:

    /src/
        bootloader.asm     ; Existing NASM bootloader
        kernel.c           ; New C kernel
        linker.ld          ; Linker script for flat binary layout
    /dist/
        os.img             ; Final bootable image for QEMU
    Makefile               ; Updated build logic

    Toolchain assumptions:

        Use nasm for assembly.

        Use gcc -m32, ld, and objcopy for the C kernel.

        Avoid external frameworks, libraries, or bootloaders like GRUB.

💡 Notes for LLM

    Ensure NASM bootloader jumps to the kernel's physical address.

    Ensure kernel uses _start as entry point or kernel_main if redirected from assembly stub.

    kernel.c should not use any libc functions; use bare metal programming style.

    Kernel message should be printed using a low-level function, e.g., directly writing to VGA text buffer at 0xB8000.

📦 LLM Output Should Include:

    src/kernel.c — a C file with kernel_main() that writes to VGA memory.

    src/linker.ld — linker script aligning the kernel correctly in memory.

    Updated Makefile — compiles bootloader.asm and kernel.c, links them, and produces dist/os.img.

You’ve previously written a minimal OS bootloader and kernel (NASM + C), and placed all code inside /src/. Refactor the project to follow a modular, scalable directory structure as shown below:
📁 Target Directory Structure

/vibexos/
│
├── Makefile                     # Updated Makefile reflecting new structure
│
├── /src/
│   ├── /boot/                   # NASM bootloader
│   │   └── bootloader.asm
│   │
│   ├── /kernel/                 # C kernel code
│   │   └── kernel.c
│   │
│   └── /linker/                 # Linker script
│       └── linker.ld
│
├── /build/                      # Object files, intermediate binaries
│   ├── boot.o
│   ├── kernel.o
│   └── kernel.bin
│
├── /dist/                       # Final bootable image
│   └── os.img

🧾 Instructions for the LLM

    Move files:

        Move bootloader.asm into /src/boot/

        Move kernel.c into /src/kernel/

        Move linker.ld into /src/linker/

    Refactor the Makefile:

        Update all source paths to reflect the new structure.

        Create /build and /dist automatically if they don't exist.

        Compile assembly and C files into /build/*.o.

        Link kernel to /build/kernel.bin.

        Concatenate bootloader + kernel binary into /dist/os.img.

    No code logic needs to change, just move and rewire the build paths.

    Optional: Add short comments in Makefile to explain what each step does for readability.