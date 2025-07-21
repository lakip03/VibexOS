#include "../include/keyboard.h"
#include "../include/printf.h"

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_start = 0;
static int buffer_end = 0;

// US QWERTY scancode to ASCII translation table (for scancodes 0x01-0x58)
static char scancode_to_ascii[] = {
    0,    0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*',  0,   ' '
};

static unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile("inb %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

static void add_to_buffer(char c) {
    int next_end = (buffer_end + 1) % KEYBOARD_BUFFER_SIZE;
    if (next_end != buffer_start) {
        keyboard_buffer[buffer_end] = c;
        buffer_end = next_end;
    }
}

void keyboard_init(void) {
    buffer_start = 0;
    buffer_end = 0;
}

void keyboard_handler(void) {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    
    // Only handle key press events (bit 7 clear)
    if (!(scancode & 0x80)) {
        // Convert scancode to ASCII if it's in our table
        if (scancode < sizeof(scancode_to_ascii) && scancode_to_ascii[scancode] != 0) {
            char ascii = scancode_to_ascii[scancode];
            add_to_buffer(ascii);
        }
    }
}

char get_key_from_buffer(void) {
    if (buffer_start == buffer_end) {
        return 0;  // Buffer empty
    }
    
    char key = keyboard_buffer[buffer_start];
    buffer_start = (buffer_start + 1) % KEYBOARD_BUFFER_SIZE;
    return key;
}

int is_key_available(void) {
    return buffer_start != buffer_end;
}