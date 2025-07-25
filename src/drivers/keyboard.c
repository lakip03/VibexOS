#include "../include/keyboard.h"
#include "../include/printf.h"

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_start = 0;
static int buffer_end = 0;
static int shift_pressed = 0;

// US QWERTY scancode to ASCII translation table (for scancodes 0x01-0x58)
static char scancode_to_ascii[] = {
    0,    0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*',  0,   ' '
};

// Shift-modified characters for numbers and symbols
static char scancode_to_shift_ascii[] = {
    0,    0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0,    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*',  0,   ' '
};

#define SCANCODE_LSHIFT 0x2A
#define SCANCODE_RSHIFT 0x36

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
    shift_pressed = 0;
}

void keyboard_handler(void) {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    
    // Check for key release (bit 7 set)
    if (scancode & 0x80) {
        // Key release
        scancode &= 0x7F;  // Remove the release bit
        
        // Handle shift key release
        if (scancode == SCANCODE_LSHIFT || scancode == SCANCODE_RSHIFT) {
            shift_pressed = 0;
        }
    } else {
        // Key press
        
        // Handle shift key press
        if (scancode == SCANCODE_LSHIFT || scancode == SCANCODE_RSHIFT) {
            shift_pressed = 1;
            return;  // Don't add shift to buffer
        }
        
        // Convert scancode to ASCII if it's in our table
        if (scancode < sizeof(scancode_to_ascii)) {
            char ascii;
            
            if (shift_pressed && scancode < sizeof(scancode_to_shift_ascii) && scancode_to_shift_ascii[scancode] != 0) {
                ascii = scancode_to_shift_ascii[scancode];
            } else if (scancode_to_ascii[scancode] != 0) {
                ascii = scancode_to_ascii[scancode];
            } else {
                return;  // No valid character mapping
            }
            
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