#include "../include/screen.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static volatile unsigned char *video_memory = (volatile unsigned char*)0xB8000;
extern int cursor_x;
extern int cursor_y;

void clearscr(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = 0x07;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void gotoxy(int x, int y) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        cursor_x = x;
        cursor_y = y;
    }
}

void set_cursor_position(int x, int y) {
    gotoxy(x, y);
    update_hardware_cursor(x, y);
}

static void outb(unsigned short port, unsigned char value) {
    asm volatile("outb %0, %1" : : "a"(value), "dN"(port));
}

void update_hardware_cursor(int x, int y) {
    unsigned short pos = y * VGA_WIDTH + x;
    
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}