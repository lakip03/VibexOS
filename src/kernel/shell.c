#include "../include/shell.h"
#include "../include/keyboard.h"
#include "../include/printf.h"

static char command_buffer[SHELL_BUFFER_SIZE];
static int buffer_pos = 0;

void shell_init(void) {
    buffer_pos = 0;
    printf("> ");
}

void shell_run(void) {
    while (1) {
        if (is_key_available()) {
            char key = get_key_from_buffer();
            
            if (key == '\n') {
                // Enter pressed - process command
                command_buffer[buffer_pos] = '\0';
                printf("\nYou typed: %s\n", command_buffer);
                
                // Reset buffer for next command
                buffer_pos = 0;
                printf("> ");
            } else if (key == '\b') {
                // Backspace
                if (buffer_pos > 0) {
                    buffer_pos--;
                    // Move cursor back, print space, move back again
                    printf("\b \b");
                }
            } else if (key >= 32 && key <= 126) {
                // Printable ASCII character
                if (buffer_pos < SHELL_BUFFER_SIZE - 1) {
                    command_buffer[buffer_pos++] = key;
                    printf("%c", key);
                }
            }
        }
        
        // Halt to save CPU cycles
        asm volatile("hlt");
    }
}