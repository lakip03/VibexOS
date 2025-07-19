#include "../include/printf.h"

void _start(void) {
    // Clear screen first
    volatile unsigned short *video = (volatile unsigned short*)0xB8000;
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = 0x0720;
    }
    
    // Test printf functionality
    printf("Welcome to VibexOS version %d\n", 1);
    printf("32-bit kernel running!\n");
    printf("Printf library fully functional!\n");
    printf("Character test: %c\n", 'A');
    printf("String test: %s\n", "Hello World");
    printf("Hex test: 0x%x\n", 0xDEAD);
    printf("System halted. Press reset to restart.\n");
    
    // Proper halt - disable interrupts and halt once
    __asm__ volatile("cli");
    __asm__ volatile("hlt");
    
    // If we somehow continue, just return
    return;
}