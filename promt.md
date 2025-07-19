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