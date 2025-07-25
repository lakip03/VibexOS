#include "../include/shell.h"
#include "../include/keyboard.h"
#include "../include/printf.h"
#include "../include/vga.h"
#include "../include/kscanf.h"
#include "../include/ramfs.h"

static char command_buffer[SHELL_BUFFER_SIZE];
static int buffer_pos = 0;

static int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

static int strncmp(const char *str1, const char *str2, int n) {
    for (int i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        if (str1[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

static void extract_argument(const char *command, char *arg) {
    int i = 0;
    while (command[i] && command[i] != ' ') i++;
    while (command[i] && command[i] == ' ') i++;
    
    int j = 0;
    while (command[i] && command[i] != ' ' && j < 31) {
        arg[j++] = command[i++];
    }
    arg[j] = '\0';
}

void shell_init(void) {
    buffer_pos = 0;
    ramfs_init();
    printf("> ");
}

void shell_run(void) {
    while (1) {
        if (is_key_available()) {
            char key = get_key_from_buffer();
            
            if (key == '\n') {
                // Enter pressed - process command
                command_buffer[buffer_pos] = '\0';
                
                if (strcmp(command_buffer, "clear") == 0) {
                    vga_clear();
                    printf("> ");
                } else if (strcmp(command_buffer, "ls") == 0) {
                    printf("\n");
                    ramfs_list_files();
                    printf("> ");
                } else if (strncmp(command_buffer, "cat ", 4) == 0) {
                    char filename[32];
                    extract_argument(command_buffer, filename);
                    printf("\n");
                    if (filename[0] == '\0') {
                        printf("Usage: cat <filename>\n");
                    } else {
                        ramfs_read_file(filename);
                    }
                    printf("\n> ");
                } else if (strcmp(command_buffer, "test") == 0) {
                    char name[32];
                    int age;
                    printf("\nEnter your name: ");
                    kscanf("%s", name);
                    printf("Enter your age: ");
                    kscanf("%d", &age);
                    printf("Hello, %s. You are %d years old.\n", name, age);
                    printf("> ");
                } else {
                    printf("\nYou typed: %s\n", command_buffer);
                    printf("> ");
                }
                
                // Reset buffer for next command
                buffer_pos = 0;
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