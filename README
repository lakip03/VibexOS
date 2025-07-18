# VibexOS 🌀
*An AI-prompted operating system built from scratch — no code written by hand.*

## 🧪 What is VibexOS?
VibexOS is a 14-day experiment to push the limits of AI-assisted software development by building an operating system entirely through **prompt engineering**.  
No code was written by hand — every function, memory layout, and assembly instruction was generated through large language models like **ChatGPT** and **Claude** using carefully crafted prompts.

This project explores:
- Whether AI can write **low-level systems code** reliably
- How far **prompt engineering** can take you in an unforgiving domain
- What it really means to “vibecode” something as complex as an OS

> ⚠️ This is *not* a tutorial. This is a real-time, documented journey — with all the bugs, breakthroughs, and AI quirks that come with it.

## 🧱 Project Structure

```
/src         → All source code (AI-generated only)
  └── boot/  → NASM-based bootloader
  └── kernel/→ Core OS code in pure C
/dist        → Compiled OS image and binaries
/docs        → AI prompts, responses, and crash logs
README.md    → You're reading it
Makefile     → Builds the .img using NASM and GCC
```

## 🎯 Challenge Goals

**Timeframe:** 14 Days  
**Scope:**
- Custom bootloader in NASM
- Monolithic x86_64 kernel
- Command-line shell with built-in text editor
- RAM-only filesystem (no disk I/O)
- Fully virtualized in QEMU (CLI only, no GUI debuggers)

**Constraints:**
- Zero human-written code
- No frameworks or libraries
- Only AI-generated assembly & C

## 🤖 The AI Workflow

To make this work, I created a multi-agent prompting setup:

### 👨‍🏫 “Jhon” — The Prompt Engineer (ChatGPT)
A custom ChatGPT persona who transforms vague ideas into **precise, technical prompts**, including:
- Memory layouts, interrupt table specs
- NASM/GCC syntax constraints
- ELF format details and boot sector limits

### 🧠 Claude — The Code Generator
Receives Jhon's prompts and outputs code for:
- Bootloaders
- Kernel memory management
- Shell input/output
- File handling in RAM

## 📷 Demo

![bootloader screenshot](docs/boot_message.png)

> “I’m sorry Dave, I’m afraid I can’t let you do that.”  
> — The first boot message from VibexOS, generated entirely by AI

## 📚 Full Log on Medium

This project is being written as a live journal on Medium:  
📰 [**Read the series: “Testing the Limits of AI Vibecoding: Building an OS From Scratch”**](https://medium.com/@your-profile-link) *(replace with actual link)*

You’ll find:
- Daily logs of progress
- Prompt iterations
- Failures, restarts, and final breakthroughs
- Lessons learned from real-world LLM prompting

## ✅ Success Criteria

By the end of Day 14, VibexOS should allow me to:
- Boot from BIOS into a custom kernel
- Drop into a command prompt
- Create, edit, and read text files in memory
- Navigate a simple RAM-based filesystem

If I can type `edit hello.txt`, write content, save, and `cat` it — that's a win.

## 📎 License

MIT License — all generated content, prompts, and tooling are free to fork, remix, and build on.  
Just please **don’t remove attribution** if you're using this as a learning resource or base project.

## 🧠 Why This Matters

This isn’t about showing off what AI can do.  
It’s about showing:
- What *you* need to know to make AI work on hard problems  
- Where AI falls short — and how human guidance fills in  
- How modern tooling is redefining software creation itself

> *Building an OS used to require arcane knowledge and weeks of pain. Now, it still kind of does — but at least now I have someone to blame.*
