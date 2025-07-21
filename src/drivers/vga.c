#include "../include/vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000
#define VGA_DEFAULT_COLOR 0x07

static volatile unsigned char *vga_buffer = (volatile unsigned char*)VGA_MEMORY;
extern int cursor_x;
extern int cursor_y;

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i * 2] = ' ';
        vga_buffer[i * 2 + 1] = VGA_DEFAULT_COLOR;
    }
    cursor_x = 0;
    cursor_y = 0;
}