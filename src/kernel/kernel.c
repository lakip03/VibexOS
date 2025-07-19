void _start(void) {
    volatile unsigned char *video_memory = (volatile unsigned char *) 0xB8000;
    const char *message = "Welcome to VibexOS";
    int i = 0;
    
    while (message[i] != '\0') {
        video_memory[i * 2] = message[i];
        video_memory[i * 2 + 1] = 0x07;
        i++;
    }
    
    while (1) {
        __asm__("hlt");
    }
}