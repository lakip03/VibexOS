#include "../include/shell.h"
#include "../include/keyboard.h"
#include "../include/printf.h"
#include "../include/vga.h"
#include "../include/kscanf.h"
#include "../include/ramfs.h"
#include "../include/vtext.h"

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

static void strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
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

static void extract_two_arguments(const char *command, char *arg1, char *arg2) {
    int i = 0;
    while (command[i] && command[i] != ' ') i++;
    while (command[i] && command[i] == ' ') i++;
    
    int j = 0;
    while (command[i] && command[i] != ' ' && j < 31) {
        arg1[j++] = command[i++];
    }
    arg1[j] = '\0';
    
    while (command[i] && command[i] == ' ') i++;
    
    j = 0;
    while (command[i] && j < 1023) {
        arg2[j++] = command[i++];
    }
    arg2[j] = '\0';
}

void shell_init(void) {
    buffer_pos = 0;
    ramfs_init();
    
    printf("\n");
    printf("  __ __  __  __               ____   _____ \n");
    printf("  \\ V  \\/  \\/  |_  _____  ___|  O  \\ / ___/ \n");
    printf("   \\ V  /\\  /| . \\/ -_  \\/   |   _/ \\___ \\ \n");
    printf("    \\_/\\_  \\_/|___/\\___  /\\   |___|  /___/ \n");
    printf("         \\_/           \\_/  |__|           \n");
    printf("\n");
    printf("Welcome to VibexOS - A Simple Operating System\n");
    printf("Type 'help' for available commands, 'info' for system details.\n");
    printf("\nReady for commands!\n");
    printf("VibexOS> ");
}

void shell_run(void) {
    while (1) {
        if (is_key_available()) {
            char key = get_key_from_buffer();
            
            if (key == '\n') {
                // Enter pressed - process command
                command_buffer[buffer_pos] = '\0';
                
                if (strcmp(command_buffer, "") == 0) {
                    // Empty command - just show prompt
                    printf("VibexOS> ");
                } else if (strcmp(command_buffer, "help") == 0) {
                    printf("\n=== VibexOS Command Reference ===\n\n");
                    printf("File System Commands:\n");
                    printf("  ls                    - List all files in RAMFS\n");
                    printf("  cat <filename>        - Display file contents\n");
                    printf("  touch <file> <text>   - Create/update file with content\n");
                    printf("  vtext <filename>      - Open text editor (F12=Save, F4=Exit)\n");
                    printf("\nSystem Commands:\n");
                    printf("  clear                 - Clear the screen\n");
                    printf("  help                  - Show this help message\n");
                    printf("  info                  - Display system information\n");
                    printf("  test                  - Interactive input test\n");
                    printf("\nText Editor (vtext) Controls:\n");
                    printf("  Arrow Keys            - Navigate cursor\n");
                    printf("  F12                   - Save file\n");
                    printf("  F4                    - Exit editor\n");
                    printf("  Enter                 - New line\n");
                    printf("  Backspace             - Delete character\n");
                    printf("\nTip: Type a command and press Enter to execute it.\n");
                    printf("     Files are stored in RAM and lost on reboot.\n\n");
                    printf("VibexOS> ");
                } else if (strcmp(command_buffer, "clear") == 0) {
                    vga_clear();
                    printf("VibexOS> ");
                } else if (strcmp(command_buffer, "ls") == 0) {
                    printf("\n");
                    ramfs_list_files();
                    printf("VibexOS> ");
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
                } else if (strncmp(command_buffer, "touch ", 6) == 0) {
                    char filename[32];
                    char content[1024];
                    extract_two_arguments(command_buffer, filename, content);
                    printf("\n");
                    if (filename[0] == '\0') {
                        printf("Usage: touch <filename> <content>\n");
                    } else {
                        if (content[0] == '\0') {
                            strcpy(content, "");
                        }
                        int result = ramfs_write_file(filename, content);
                        if (result == 0) {
                            printf("File '%s' created/updated successfully\n", filename);
                        } else {
                            printf("Failed to create/update file '%s'\n", filename);
                        }
                    }
                    printf("VibexOS> ");
                } else if (strncmp(command_buffer, "vtext ", 6) == 0) {
                    char filename[32];
                    extract_argument(command_buffer, filename);
                    if (filename[0] == '\0') {
                        printf("\nUsage: vtext <filename>\n");
                        printf("VibexOS> ");
                    } else {
                        vtext_editor editor;
                        vtext_init(&editor, filename);
                        vtext_run(&editor);
                        vga_clear();
                        printf("VibexOS> ");
                    }
                } else if (strcmp(command_buffer, "info") == 0) {
                    printf("\n=== VibexOS System Information ===\n\n");
                    printf("Operating System: VibexOS v1.0\n");
                    printf("Architecture:     x86 (32-bit)\n");
                    printf("Kernel Type:      Monolithic\n");
                    printf("File System:      RAMFS (In-Memory)\n");
                    printf("Text Editor:      VText\n");
                    printf("Max Files:        64\n");
                    printf("Max File Size:    1024 bytes\n");
                    printf("Screen Mode:      VGA Text Mode (80x25)\n");
                    printf("Input:            PS/2 Keyboard\n");
                    printf("\nFeatures:\n");
                    printf("  - Real-time file editing with VText\n");
                    printf("  - Hardware cursor support\n");
                    printf("  - Function key shortcuts\n");
                    printf("  - Basic shell commands\n");
                    printf("  - In-memory file system\n\n");
                    printf("VibexOS> ");
                } else if (strcmp(command_buffer, "test") == 0) {
                    char name[32];
                    int age;
                    printf("\nEnter your name: ");
                    kscanf("%s", name);
                    printf("Enter your age: ");
                    kscanf("%d", &age);
                    printf("Hello, %s. You are %d years old.\n", name, age);
                    printf("VibexOS> ");
                } else {
                    printf("\nUnknown command: '%s'\n", command_buffer);
                    
                    // Suggest similar commands
                    if (strcmp(command_buffer, "list") == 0 || strcmp(command_buffer, "dir") == 0) {
                        printf("Did you mean 'ls'? (lists files)\n");
                    } else if (strcmp(command_buffer, "edit") == 0 || strcmp(command_buffer, "nano") == 0 || strcmp(command_buffer, "vim") == 0) {
                        printf("Did you mean 'vtext <filename>'? (text editor)\n");
                    } else if (strcmp(command_buffer, "read") == 0 || strcmp(command_buffer, "type") == 0 || strcmp(command_buffer, "show") == 0) {
                        printf("Did you mean 'cat <filename>'? (display file)\n");
                    } else if (strcmp(command_buffer, "create") == 0 || strcmp(command_buffer, "make") == 0 || strcmp(command_buffer, "new") == 0) {
                        printf("Did you mean 'touch <filename> <content>'? (create file)\n");
                    } else if (strcmp(command_buffer, "cls") == 0 || strcmp(command_buffer, "clr") == 0) {
                        printf("Did you mean 'clear'? (clear screen)\n");
                    }
                    
                    printf("Type 'help' for a list of available commands.\n");
                    printf("VibexOS> ");
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