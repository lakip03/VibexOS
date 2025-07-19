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

# Files
BOOTLOADER_SRC = $(SRC_DIR)/boot/simple_bootloader.asm
SIMPLE_KERNEL_SRC = $(SRC_DIR)/kernel/simple_kernel.asm
KERNEL_LINKER_SCRIPT = $(SRC_DIR)/linker/linker.ld
BOOTLOADER_BIN = $(BUILD_DIR)/bootloader.bin
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMG = $(DIST_DIR)/os.img

# Default target
all: $(OS_IMG)

# Build the bootable OS image
$(OS_IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN) | $(DIST_DIR)
	cat $(BOOTLOADER_BIN) > $(OS_IMG)
	dd if=$(KERNEL_BIN) of=$(OS_IMG) bs=512 seek=1 conv=notrunc 2>/dev/null
	truncate -s 1440K $(OS_IMG)
	@echo "OS image built successfully: $(OS_IMG)"

# Build bootloader
$(BOOTLOADER_BIN): $(BOOTLOADER_SRC) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $(BOOTLOADER_SRC) -o $(BOOTLOADER_BIN)
	@echo "Bootloader built: $(BOOTLOADER_BIN)"

# Build simple kernel (pure assembly, no linking needed)
$(KERNEL_BIN): $(SIMPLE_KERNEL_SRC) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $(SIMPLE_KERNEL_SRC) -o $(KERNEL_BIN)
	@echo "Simple kernel built: $(KERNEL_BIN)"

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

# Check if bootloader is exactly 512 bytes
check-bootloader-size: $(BOOTLOADER_BIN)
	@SIZE=$$(stat -c%s $(BOOTLOADER_BIN)); \
	if [ $$SIZE -eq 512 ]; then \
		echo "✓ Bootloader size is correct: $$SIZE bytes"; \
	else \
		echo "✗ Bootloader size is incorrect: $$SIZE bytes (should be 512)"; \
		exit 1; \
	fi

# Verify boot signature
check-signature: $(BOOTLOADER_BIN)
	@SIGNATURE=$$(xxd -l 2 -s 510 $(BOOTLOADER_BIN) | awk '{print $$2}'); \
	if [ "$$SIGNATURE" = "55aa" ]; then \
		echo "✓ Boot signature is correct: 0xAA55"; \
	else \
		echo "✗ Boot signature is incorrect: 0x$$SIGNATURE (should be 0xAA55)"; \
		exit 1; \
	fi

# Verify the bootloader
verify: check-bootloader-size check-signature
	@echo "✓ Bootloader verification passed"

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