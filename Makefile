# Makefile for VibexOS Bootloader
# Builds a minimal x86_64 bootloader

# Assembler
ASM = nasm
ASMFLAGS = -f bin

# Source and output directories
SRC_DIR = src
DIST_DIR = dist

# Files
BOOTLOADER_SRC = $(SRC_DIR)/bootloader.asm
BOOTLOADER_IMG = $(DIST_DIR)/os.img

# Default target
all: $(BOOTLOADER_IMG)

# Build the bootloader image
$(BOOTLOADER_IMG): $(BOOTLOADER_SRC) | $(DIST_DIR)
	$(ASM) $(ASMFLAGS) $(BOOTLOADER_SRC) -o $(BOOTLOADER_IMG)
	@echo "Bootloader built successfully: $(BOOTLOADER_IMG)"

# Ensure dist directory exists
$(DIST_DIR):
	mkdir -p $(DIST_DIR)

# Clean build artifacts
clean:
	rm -rf $(DIST_DIR)
	@echo "Cleaned build artifacts"

# Run the bootloader in QEMU
run: $(BOOTLOADER_IMG)
	qemu-system-x86_64 -drive format=raw,file=$(BOOTLOADER_IMG) -boot c

# Run with legacy BIOS mode explicitly
run-bios: $(BOOTLOADER_IMG)
	qemu-system-x86_64 -drive format=raw,file=$(BOOTLOADER_IMG) -boot c -machine pc

# Check if bootloader is exactly 512 bytes
check-size: $(BOOTLOADER_IMG)
	@SIZE=$$(stat -c%s $(BOOTLOADER_IMG)); \
	if [ $$SIZE -eq 512 ]; then \
		echo "✓ Bootloader size is correct: $$SIZE bytes"; \
	else \
		echo "✗ Bootloader size is incorrect: $$SIZE bytes (should be 512)"; \
		exit 1; \
	fi

# Verify boot signature
check-signature: $(BOOTLOADER_IMG)
	@SIGNATURE=$$(xxd -l 2 -s 510 $(BOOTLOADER_IMG) | awk '{print $$2}'); \
	if [ "$$SIGNATURE" = "55aa" ]; then \
		echo "✓ Boot signature is correct: 0xAA55"; \
	else \
		echo "✗ Boot signature is incorrect: 0x$$SIGNATURE (should be 0xAA55)"; \
		exit 1; \
	fi

# Verify the bootloader
verify: check-size check-signature
	@echo "✓ Bootloader verification passed"

# Show help
help:
	@echo "VibexOS Bootloader Build System"
	@echo "Available targets:"
	@echo "  all         - Build the bootloader (default)"
	@echo "  clean       - Remove build artifacts"
	@echo "  run         - Run bootloader in QEMU"
	@echo "  run-bios    - Run bootloader in QEMU with explicit BIOS mode"
	@echo "  check-size  - Verify bootloader is exactly 512 bytes"
	@echo "  check-signature - Verify boot signature (0xAA55)"
	@echo "  verify      - Run all verification checks"
	@echo "  help        - Show this help message"

.PHONY: all clean run run-bios check-size check-signature verify help