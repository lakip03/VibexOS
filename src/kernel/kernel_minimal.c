void kernel_main(void) {
    volatile unsigned short *video = (volatile unsigned short*)0xB8000;
    
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = 0x0720;
    }
    
    const char* msg = "Minimal kernel works!";
    for (int i = 0; msg[i] != '\0'; i++) {
        video[i] = (0x07 << 8) | msg[i];
    }
    
    while(1) {
        __asm__ volatile("hlt");
    }
}