# VibexOS 🌀
*The first AI-generated operating system with full text editing capabilities*

## Project Overview

**VibexOS** is a groundbreaking experiment in AI-powered operating system development — the first OS where every single line of code was generated through prompt engineering and AI collaboration. This project represents a major milestone in "vibecoding" methodology, proving that complex systems software can be created entirely through natural language interaction with AI.

## ✅ **Mission Accomplished!**

We have successfully achieved all our initial goals:

- ✅ **Custom Bootloader** - Multi-stage boot process with VibexOS branding
- ✅ **Interactive Shell** - Feature-rich command-line interface with help system
- ✅ **File System** - Complete RAMFS with create/read/write operations  
- ✅ **Text Editor** - Full-featured VText editor with cursor navigation
- ✅ **Hardware Integration** - PS/2 keyboard, VGA display, function keys
- ✅ **User Experience** - Intuitive interface with comprehensive help

**The ultimate test passed**: Create, edit, save, and read files entirely within VibexOS!

## 🎯 **Key Features**

### **VText Editor**
- **Full cursor navigation** with arrow keys
- **Hardware cursor** support for precise editing
- **Function key shortcuts**: F12 (Save), F4 (Exit)
- **Multi-line editing** with proper text wrapping
- **Real-time status** display (line/column, modified state)

### **Advanced Shell**
- **Smart command suggestions** for typos
- **Comprehensive help system** (`help` command)
- **System information** (`info` command)  
- **File operations**: `ls`, `cat`, `touch`, `vtext`
- **Enhanced prompts** and error feedback

### **RAMFS File System**
- **64 files maximum**, 1024 bytes each
- **Real-time file operations** 
- **Pre-loaded examples** and documentation
- **Complete persistence** during session

## 🚀 **Quick Start**

### **Build and Run**
```bash
# Build VibexOS
make clean && make

# Run in QEMU  
make run

# Expected output:
#   __ __  __  __               ____   _____ 
#   \ V  \/  \/  |_  _____  ___|  O  \ / ___/ 
#    \ V  /\  /| . \/ -_  \/   |   _/ \___ \ 
#     \_/\_  \_/|___/\___  /\   |___|  /___/ 
#          \_/           \_/  |__|           
# 
# Welcome to VibexOS - A Simple Operating System
# Type 'help' for available commands, 'info' for system details.
# 
# Ready for commands!
# VibexOS> _
```

### **Try These Commands**
```bash
VibexOS> help              # Complete command reference
VibexOS> ls                # List available files  
VibexOS> cat readme.txt    # Read the welcome guide
VibexOS> vtext myfile.txt  # Create and edit files
VibexOS> info              # System specifications
```

## 🛠 **Installation Requirements**

### **Linux (Arch)**
```bash
sudo pacman -S nasm gcc qemu-system-x86 make
git clone https://github.com/lakip03/VibexOS.git
cd VibexOS
make run
```

### **Linux (Debian/Ubuntu)**
```bash
sudo apt update
sudo apt install nasm gcc-multilib qemu-system-x86 make
git clone https://github.com/lakip03/VibexOS.git
cd VibexOS
make run
```

### **macOS**
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install nasm qemu make
# For GCC cross-compilation
brew install i686-elf-gcc

git clone https://github.com/lakip03/VibexOS.git
cd VibexOS
make run
```

### **Windows**
```powershell
# Install using Chocolatey
choco install qemu nasm make mingw

# Or use WSL2 (Recommended)
wsl --install
# Then follow Linux (Debian) instructions inside WSL

# Clone and build
git clone https://github.com/lakip03/VibexOS.git
cd VibexOS
make run
```

### **Alternative: Docker Setup**
```bash
# Cross-platform solution
docker build -t vibexos .
docker run -it --rm vibexos
```

## 🏗 **Architecture**

```
VibexOS Complete System:
┌─────────────────────────────┐
│      Stage 1 Bootloader     │ ← 512-byte boot sector
├─────────────────────────────┤
│      Stage 2 Bootloader     │ ← Extended boot features  
├─────────────────────────────┤
│         Kernel Core         │
│ ┌─────────────────────────┐ │
│ │    Interactive Shell    │ │ ← Command processing
│ │   - Help system         │ │
│ │   - Smart suggestions   │ │
│ └─────────────────────────┘ │
│ ┌─────────────────────────┐ │
│ │    VText Editor         │ │ ← Full text editing
│ │   - Cursor navigation   │ │
│ │   - Function keys       │ │
│ │   - Multi-line support  │ │
│ └─────────────────────────┘ │
│ ┌─────────────────────────┐ │
│ │     RAMFS Driver        │ │ ← File system
│ │   - 64 files max        │ │
│ │   - 1024 bytes each     │ │
│ └─────────────────────────┘ │
│ ┌─────────────────────────┐ │
│ │   Hardware Drivers      │ │ ← PS/2, VGA, etc.
│ └─────────────────────────┘ │
└─────────────────────────────┘
```

## 🤖 **The AI Development Process**

This project represents a new paradigm in software development:

### **Pure AI Generation**
- **Zero human-written code** - Every line generated through prompts
- **Natural language specs** → Working assembly and C code
- **Iterative refinement** through AI conversation
- **Complex system integration** via prompt engineering

### **Vibecoding Methodology**
1. **Conceptual prompting** - Define what the system should do
2. **Technical translation** - AI converts requirements to implementation
3. **Code generation** - AI writes bootloader, kernel, and applications  
4. **Integration testing** - AI debugs and refines the system
5. **UX enhancement** - AI improves user experience and documentation

## 📊 **Technical Specifications**

- **Architecture**: x86 (32-bit)
- **Boot Method**: Custom multi-stage bootloader
- **Kernel Type**: Monolithic
- **File System**: RAMFS (in-memory)
- **Text Mode**: VGA 80x25 color
- **Input**: PS/2 keyboard with function key support
- **Memory Model**: Static allocation
- **Build System**: GNU Make with GCC and NASM

## 📁 **Project Structure**

```
VibexOS/
├── src/
│   ├── boot/
│   │   ├── stage1.asm          # Boot sector (512 bytes)
│   │   └── stage2.asm          # Extended bootloader
│   ├── kernel/
│   │   ├── kernel.c            # Main kernel
│   │   ├── shell.c             # Interactive shell
│   │   └── idt.c               # Interrupt handling
│   ├── drivers/
│   │   ├── keyboard.c          # PS/2 keyboard + function keys
│   │   └── vga.c               # VGA text mode
│   ├── lib/
│   │   ├── vtext.c             # Text editor
│   │   ├── ramfs.c             # File system
│   │   ├── printf.c            # Formatted output
│   │   └── screen.c            # Hardware cursor
│   └── include/                # Header files
├── promt-history/              # AI conversation logs
├── Makefile                    # Build configuration
└── README.md                   # This file
```

## 🎉 **Final Words from the Creator**

So this is it for now! We have achieved all of our goals and can read/write/edit files now! This is a major win for vibecoding in general… I will write a RECAP with more clear and better information about what, why and how. This two parts will serve as a diary more then anything.

For the future of VibexOS I will try to add more features from time to time and as technology progress and also if anyone for some reason want's you can always contribute code to my GitHub repo, or just fork it and make it into something better, or even better rewrite it in rust.

Follow my medium and LinkedIn and GitHub for future projects and write ups!

## 🔗 **Connect & Contribute**

- **GitHub**: Fork, contribute, or rewrite in Rust!
- **Medium**: Follow for detailed writeups and tutorials
- **LinkedIn**: Connect for professional updates
- **Issues**: Report bugs or suggest features

## 📜 **License**

MIT License - Feel free to fork, modify, and improve!

---

*VibexOS v1.0 - Proving that the future of programming might just be having better conversations with AI.* 🚀