#include "../include/printf.h"

void kernel_main(void) {
    // Simple direct video memory test first
    volatile unsigned short *video = (volatile unsigned short*)0xB8000;
    
    // Clear screen
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = 0x0720; // Space with white text on black background
    }
    
    // Write directly to video memory as backup
    const char* msg = "VibexOS C Kernel v2.0";
    for (int i = 0; msg[i] != '\0'; i++) {
        video[i] = (0x07 << 8) | msg[i];
    }
    
    // Test printf functionality
    printf("\nBootloader -> C Kernel handoff: SUCCESS\n");
    printf("Direct video memory: WORKING\n");
    
    
    // Keep kernel running
    while(1) {
        __asm__ volatile("hlt");
    }
}