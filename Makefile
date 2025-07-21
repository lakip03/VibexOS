# Makefile for VibexOS
# Builds a minimal x86_64 bootloader and kernel

# Assembler and compiler
ASM = nasm
ASMFLAGS = -f bin
CC = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -fno-pic -I $(SRC_DIR)/include
LD = ld
LDFLAGS = -T $(LINKER_SCRIPT)

# Source and output directories
SRC_DIR = src
BUILD_DIR = build
DIST_DIR = dist

# Files - Multi-stage Bootloader Edition
STAGE1_SRC = $(SRC_DIR)/boot/stage1.asm
STAGE2_SRC = $(SRC_DIR)/boot/stage2.asm
KERNEL_ENTRY_SRC = $(SRC_DIR)/kernel/entry.asm
KERNEL_SRC = $(SRC_DIR)/kernel/kernel.c
PRINTF_SRC = $(SRC_DIR)/lib/printf.c
SCREEN_SRC = $(SRC_DIR)/lib/screen.c
KSCANF_SRC = $(SRC_DIR)/lib/kscanf.c
IDT_SRC = $(SRC_DIR)/kernel/idt.c
IDT_ASM_SRC = $(SRC_DIR)/kernel/idt_asm.asm
KEYBOARD_SRC = $(SRC_DIR)/drivers/keyboard.c
VGA_SRC = $(SRC_DIR)/drivers/vga.c
SHELL_SRC = $(SRC_DIR)/kernel/shell.c
KERNEL_LINKER_SCRIPT = $(SRC_DIR)/linker/linker.ld
STAGE1_BIN = $(BUILD_DIR)/stage1.bin
STAGE2_BIN = $(BUILD_DIR)/stage2.bin
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/entry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
PRINTF_OBJ = $(BUILD_DIR)/printf.o
SCREEN_OBJ = $(BUILD_DIR)/screen.o
KSCANF_OBJ = $(BUILD_DIR)/kscanf.o
IDT_OBJ = $(BUILD_DIR)/idt.o
IDT_ASM_OBJ = $(BUILD_DIR)/idt_asm.o
KEYBOARD_OBJ = $(BUILD_DIR)/keyboard.o
VGA_OBJ = $(BUILD_DIR)/vga.o
SHELL_OBJ = $(BUILD_DIR)/shell.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMG = $(DIST_DIR)/os.img

# Default target
all: $(OS_IMG)

# Build the bootable OS image
$(OS_IMG): $(STAGE1_BIN) $(STAGE2_BIN) $(KERNEL_BIN) | $(DIST_DIR)
	cat $(STAGE1_BIN) > $(OS_IMG)
	dd if=$(STAGE2_BIN) of=$(OS_IMG) bs=512 seek=1 conv=notrunc 2>/dev/null
	dd if=$(KERNEL_BIN) of=$(OS_IMG) bs=512 seek=9 conv=notrunc 2>/dev/null
	truncate -s 1440K $(OS_IMG)
	@echo "Multi-stage OS image built successfully: $(OS_IMG)"

# Build stage 1 bootloader
$(STAGE1_BIN): $(STAGE1_SRC) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $(STAGE1_SRC) -o $(STAGE1_BIN)
	@echo "Stage 1 bootloader built: $(STAGE1_BIN)"

# Build stage 2 bootloader
$(STAGE2_BIN): $(STAGE2_SRC) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $(STAGE2_SRC) -o $(STAGE2_BIN)
	@echo "Stage 2 bootloader built: $(STAGE2_BIN)"

# Compile kernel entry assembly
$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_SRC) | $(BUILD_DIR)
	$(ASM) -f elf32 $(KERNEL_ENTRY_SRC) -o $(KERNEL_ENTRY_OBJ)
	@echo "Kernel entry assembly compiled: $(KERNEL_ENTRY_OBJ)"

# Compile kernel C code
$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(KERNEL_SRC) -o $(KERNEL_OBJ)
	@echo "Kernel C code compiled: $(KERNEL_OBJ)"

# Compile printf library
$(PRINTF_OBJ): $(PRINTF_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(PRINTF_SRC) -o $(PRINTF_OBJ)
	@echo "Printf library compiled: $(PRINTF_OBJ)"

# Compile screen library
$(SCREEN_OBJ): $(SCREEN_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SCREEN_SRC) -o $(SCREEN_OBJ)
	@echo "Screen library compiled: $(SCREEN_OBJ)"

# Compile kscanf library
$(KSCANF_OBJ): $(KSCANF_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(KSCANF_SRC) -o $(KSCANF_OBJ)
	@echo "Kscanf library compiled: $(KSCANF_OBJ)"

# Compile IDT
$(IDT_OBJ): $(IDT_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(IDT_SRC) -o $(IDT_OBJ)
	@echo "IDT compiled: $(IDT_OBJ)"

# Compile IDT assembly
$(IDT_ASM_OBJ): $(IDT_ASM_SRC) | $(BUILD_DIR)
	$(ASM) -f elf32 $(IDT_ASM_SRC) -o $(IDT_ASM_OBJ)
	@echo "IDT assembly compiled: $(IDT_ASM_OBJ)"

# Compile keyboard driver
$(KEYBOARD_OBJ): $(KEYBOARD_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(KEYBOARD_SRC) -o $(KEYBOARD_OBJ)
	@echo "Keyboard driver compiled: $(KEYBOARD_OBJ)"

# Compile VGA driver
$(VGA_OBJ): $(VGA_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(VGA_SRC) -o $(VGA_OBJ)
	@echo "VGA driver compiled: $(VGA_OBJ)"

# Compile shell
$(SHELL_OBJ): $(SHELL_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SHELL_SRC) -o $(SHELL_OBJ)
	@echo "Shell compiled: $(SHELL_OBJ)"

# Link kernel
$(KERNEL_BIN): $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(PRINTF_OBJ) $(SCREEN_OBJ) $(KSCANF_OBJ) $(IDT_OBJ) $(IDT_ASM_OBJ) $(KEYBOARD_OBJ) $(VGA_OBJ) $(SHELL_OBJ) $(KERNEL_LINKER_SCRIPT) | $(BUILD_DIR)
	$(LD) -T $(KERNEL_LINKER_SCRIPT) $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(PRINTF_OBJ) $(SCREEN_OBJ) $(KSCANF_OBJ) $(IDT_OBJ) $(IDT_ASM_OBJ) $(KEYBOARD_OBJ) $(VGA_OBJ) $(SHELL_OBJ) -o $(KERNEL_BIN)
	@echo "Kernel linked: $(KERNEL_BIN)"

# Ensure build and dist directories exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
	@echo "Cleaned build artifacts"

# Run the OS in QEMU
run: $(OS_IMG)
	qemu-system-x86_64 -drive format=raw,file=$(OS_IMG) -boot c

# Run with legacy BIOS mode explicitly
run-bios: $(OS_IMG)
	qemu-system-x86_64 -drive format=raw,file=$(OS_IMG) -boot c -machine pc

# Check if stage 1 bootloader is exactly 512 bytes
check-stage1-size: $(STAGE1_BIN)
	@SIZE=$$(stat -c%s $(STAGE1_BIN)); \
	if [ $$SIZE -eq 512 ]; then \
		echo "✓ Stage 1 bootloader size is correct: $$SIZE bytes"; \
	else \
		echo "✗ Stage 1 bootloader size is incorrect: $$SIZE bytes (should be 512)"; \
		exit 1; \
	fi

# Verify boot signature
check-signature: $(STAGE1_BIN)
	@SIGNATURE=$$(hexdump -s 510 -n 2 -e '/1 "%02x"' $(STAGE1_BIN)); \
	if [ "$$SIGNATURE" = "55aa" ]; then \
		echo "✓ Boot signature is correct: 0xAA55"; \
	else \
		echo "✗ Boot signature is incorrect: 0x$$SIGNATURE (should be 0xAA55)"; \
		exit 1; \
	fi

# Verify the bootloader
verify: check-stage1-size check-signature
	@echo "✓ Multi-stage bootloader verification passed"

# Show help
help:
	@echo "VibexOS Build System"
	@echo "Available targets:"
	@echo "  all         - Build the OS image (default)"
	@echo "  clean       - Remove build artifacts"
	@echo "  run         - Run OS in QEMU"
	@echo "  run-bios    - Run OS in QEMU with explicit BIOS mode"
	@echo "  check-bootloader-size - Verify bootloader is exactly 512 bytes"
	@echo "  check-signature - Verify boot signature (0xAA55)"
	@echo "  verify      - Run all verification checks"
	@echo "  help        - Show this help message"

# Dependencies for proper rebuilding
$(KERNEL_BIN): $(LINKER_SCRIPT)

.PHONY: all clean run run-bios check-bootloader-size check-signature verify help