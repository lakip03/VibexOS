#include "../include/printf.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static volatile unsigned char *video_memory = (volatile unsigned char*)0xB8000;
int cursor_x = 0;
int cursor_y = 0;

int strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void scroll_screen() {
    for (int row = 0; row < VGA_HEIGHT - 1; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            int src_pos = (row + 1) * VGA_WIDTH + col;
            int dst_pos = row * VGA_WIDTH + col;
            video_memory[dst_pos * 2] = video_memory[src_pos * 2];
            video_memory[dst_pos * 2 + 1] = video_memory[src_pos * 2 + 1];
        }
    }
    
    for (int col = 0; col < VGA_WIDTH; col++) {
        int pos = (VGA_HEIGHT - 1) * VGA_WIDTH + col;
        video_memory[pos * 2] = ' ';
        video_memory[pos * 2 + 1] = 0x07;
    }
}

void print_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            scroll_screen();
            cursor_y = VGA_HEIGHT - 1;
        }
        return;
    }
    
    if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        }
        return;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            scroll_screen();
            cursor_y = VGA_HEIGHT - 1;
        }
    }
    
    int position = cursor_y * VGA_WIDTH + cursor_x;
    video_memory[position * 2] = c;
    video_memory[position * 2 + 1] = 0x07;
    cursor_x++;
}

void putchar(char c) {
    print_char(c);
}

void print_string(const char *str) {
    while (*str) {
        print_char(*str);
        str++;
    }
}

void print_int(int num) {
    if (num == 0) {
        print_char('0');
        return;
    }
    
    if (num < 0) {
        print_char('-');
        num = -num;
    }
    
    char buffer[12];
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    while (i > 0) {
        print_char(buffer[--i]);
    }
}

void print_hex(unsigned int num) {
    if (num == 0) {
        print_char('0');
        return;
    }
    
    char hex_chars[] = "0123456789abcdef";
    char buffer[9];
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = hex_chars[num % 16];
        num /= 16;
    }
    
    while (i > 0) {
        print_char(buffer[--i]);
    }
}

char *itoa(int value, char *buffer, int base) {
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }
    
    int i = 0;
    int is_negative = 0;
    
    if (value < 0 && base == 10) {
        is_negative = 1;
        value = -value;
    }
    
    while (value > 0) {
        int remainder = value % base;
        buffer[i++] = (remainder < 10) ? '0' + remainder : 'a' + remainder - 10;
        value /= base;
    }
    
    if (is_negative) {
        buffer[i++] = '-';
    }
    
    buffer[i] = '\0';
    
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    
    return buffer;
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    const char *ptr = format;
    
    while (*ptr) {
        if (*ptr == '%' && *(ptr + 1)) {
            ptr++;
            switch (*ptr) {
                case 's': {
                    char *str = va_arg(args, char*);
                    if (str) {
                        print_string(str);
                    } else {
                        print_string("(null)");
                    }
                    break;
                }
                case 'c':
                    print_char((char)va_arg(args, int));
                    break;
                case 'd':
                case 'i':
                    print_int(va_arg(args, int));
                    break;
                case 'x':
                    print_hex(va_arg(args, unsigned int));
                    break;
                case '%':
                    print_char('%');
                    break;
                default:
                    print_char('%');
                    print_char(*ptr);
                    break;
            }
        } else {
            print_char(*ptr);
        }
        ptr++;
    }
    
    va_end(args);
}