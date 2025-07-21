#include "../include/printf.h"
#include "../include/screen.h"
#include "../include/idt.h"
#include "../include/keyboard.h"
#include "../include/shell.h"

void kernel_main(void) {
    clearscr();

    printf("Welcome to VibexOS - World's First Vibecoded OS!\n");
    printf("Ready to vibe in protected mode. Let's go!\n");
    printf("Initializing keyboard and shell...\n");

    // Initialize interrupt handling and keyboard
    idt_init();
    keyboard_init();
    
    // Enable interrupts
    asm volatile("sti");
    
    printf("Shell ready! Type commands and press Enter:\n");
    shell_init();
    shell_run();
}