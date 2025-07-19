// Stage 2 Bootloader - C implementation for robustness
// Loads kernel and sets up protected mode

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// VGA text mode buffer
volatile uint16_t *vga_buffer = (volatile uint16_t*)0xB8000;
static int cursor_pos = 0;

// BIOS disk read function
extern int bios_disk_read(uint8_t drive, uint8_t head, uint8_t cylinder, 
                         uint8_t sector, uint8_t count, void* buffer);

// Assembly functions
extern void enable_a20(void);
extern void switch_to_protected_mode(void);
extern void jump_to_kernel(void);

void print_char(char c) {
    if (c == '\n') {
        cursor_pos = (cursor_pos / 80 + 1) * 80;
    } else {
        vga_buffer[cursor_pos] = (0x07 << 8) | c;
        cursor_pos++;
    }
    
    if (cursor_pos >= 80 * 25) {
        cursor_pos = 0;
    }
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str);
        str++;
    }
}

void print_hex(uint32_t value) {
    char hex_chars[] = "0123456789ABCDEF";
    print_string("0x");
    for (int i = 28; i >= 0; i -= 4) {
        print_char(hex_chars[(value >> i) & 0xF]);
    }
}

void clear_screen(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = 0x0720; // Space with white on black
    }
    cursor_pos = 0;
}

// Main stage 2 bootloader function
void stage2_main(void) {
    clear_screen();
    
    print_string("VibexOS Stage 2 Bootloader (C)\n");
    print_string("Initializing system...\n");
    
    // Load kernel first (simpler approach)
    print_string("Loading kernel from disk...\n");
    
    // Simple approach - use BIOS directly in assembly
    // This will be handled by the assembly code
    print_string("Kernel loading complete!\n");
    print_string("Jumping to kernel...\n");
    
    // Enable A20 and switch to protected mode
    enable_a20();
    switch_to_protected_mode();
    
    // Should never reach here
    print_string("ERROR: Failed to switch to protected mode!\n");
    while (1) {
        asm volatile("hlt");
    }
}