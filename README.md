# VibexOS 🌀

*An experimental operating system built entirely through AI prompt engineering.*

## 🧪 What is VibexOS?

VibexOS is an ambitious experiment to build an operating system from scratch using only AI-generated code. Every line of assembly, C code, and system configuration was created through carefully crafted prompts to large language models like **ChatGPT** and **Claude**.

This project explores:
- Whether AI can handle **low-level systems programming** reliably
- The limits and potential of **prompt engineering** in complex domains
- What it means to "AI-code" something as foundational as an operating system

> ⚠️ **Experimental Project**: This is a real-time development experiment with all the successes, failures, and AI quirks documented.

## 🎯 Project Goals

**Objective**: Create a minimal but functional operating system featuring:
- Custom x86_64 bootloader (NASM assembly)
- Monolithic kernel in C
- Command-line shell with text editor
- RAM-based filesystem (no disk I/O)
- QEMU virtualization support

**Constraints**:
- Zero human-written code
- No external frameworks or libraries
- AI-generated assembly and C only

## 🧱 Project Structure

```
VibexOS/
├── src/
│   ├── boot/           # NASM bootloader
│   │   └── bootloader.asm
│   ├── kernel/         # Core OS kernel
│   │   └── kernel.c
│   └── linker/         # Linker script
│       └── linker.ld
├── build/              # Temporary build files
├── dist/               # Final OS image and binaries
├── promt-history/      # AI prompt logs and iterations
│   ├── genisis-promt.md
│   └── kernel-1.md
├── Makefile           # Build system
└── README.md          # This file
```

## 🤖 AI Workflow

The development process uses a multi-agent AI approach:

### 👨‍🏫 "Jhon" - The Prompt Engineer (ChatGPT)
Specialized persona that transforms high-level requirements into precise technical specifications:
- Memory layout definitions
- Assembly syntax constraints
- Boot sector and ELF format requirements
- Hardware interface specifications

### 🧠 Claude - The Code Generator
Receives technical prompts and generates:
- Bootloader assembly code
- Kernel memory management
- Shell and I/O handling
- Filesystem implementation

## 🚀 Getting Started

### Prerequisites
- Linux environment (tested on EndevourOS)
- NASM assembler
- GCC compiler
- QEMU emulator
- Make build system

### Building VibexOS

```bash
# Clone the repository
git clone <repository-url>
cd VibexOS

# Build the OS image
make

# Run in QEMU
make run
```

## 📈 Development Status

- [x] Basic bootloader
- [x] Kernel foundation
- [ ] Memory management
- [ ] Shell implementation
- [ ] Text editor
- [ ] RAM filesystem
- [ ] Command processing

## 📚 Documentation

Development logs and prompt histories are maintained in the `promt-history/` directory:
- **Genesis Prompt**: Initial system specifications
- **Kernel Development**: Core kernel implementation phases

## 🧠 Why This Matters

This project demonstrates:
- **AI Capabilities**: What modern LLMs can achieve in systems programming
- **Prompt Engineering**: How to guide AI for complex technical tasks
- **Development Process**: New paradigms for software creation
- **Limitations**: Where AI needs human guidance and oversight

## 🔬 Lessons Learned

*This section will be updated as development progresses*

- AI excels at generating boilerplate and standard patterns
- Complex system interactions require careful prompt design
- Debugging AI-generated code presents unique challenges
- Domain expertise remains crucial for effective prompting

## 📄 License

MIT License - Feel free to fork, modify, and build upon this work. Attribution appreciated for educational use.

## 🤝 Contributing

While this is primarily an AI-assisted development experiment, feedback and suggestions for prompt improvements are welcome. Please open issues for:
- Build problems
- Documentation improvements
- Prompt strategy suggestions

---

*"Building an OS used to require years of specialized knowledge. Now it requires years of specialized knowledge... and really good prompts."*