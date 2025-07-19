#include "../include/printf.h"
#include "../include/screen.h"

void kernel_main(void) {
    clearscr();

    printf("Welcome to VibexOS - World's First Vibecoded OS!\n");
    printf("Ready to vibe in protected mode. Let's go!\n");

    // Keep kernel running
    while(1) {
        __asm__ volatile("hlt");
    }
}