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
}