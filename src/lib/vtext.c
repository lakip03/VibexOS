#include "../include/vtext.h"
#include "../include/keyboard.h"
#include "../include/printf.h"
#include "../include/screen.h"
#include "../include/ramfs.h"

static int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

static void strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}


void vtext_init(vtext_editor *editor, const char *filename) {
    editor->line_count = 1;
    editor->cursor_line = 0;
    editor->cursor_col = 0;
    editor->top_line = 0;
    editor->modified = 0;
    strcpy(editor->filename, filename);
    
    for (int i = 0; i < VTEXT_MAX_LINES; i++) {
        editor->lines[i][0] = '\0';
    }
    
    vtext_load_file(editor);
}

void vtext_run(vtext_editor *editor) {
    clearscr();
    vtext_display(editor);
    
    while (1) {
        int special_key = vtext_get_special_key();
        if (special_key == SPECIAL_KEY_F4) {
            return;
        } else if (special_key == SPECIAL_KEY_F12) {
            vtext_save_file(editor);
            vtext_display(editor);
        }
        
        int arrow_key = vtext_get_arrow_key();
        if (arrow_key == SPECIAL_KEY_UP) {
            vtext_move_cursor_up(editor);
            vtext_display(editor);
        } else if (arrow_key == SPECIAL_KEY_DOWN) {
            vtext_move_cursor_down(editor);
            vtext_display(editor);
        } else if (arrow_key == SPECIAL_KEY_LEFT) {
            vtext_move_cursor_left(editor);
            vtext_display(editor);
        } else if (arrow_key == SPECIAL_KEY_RIGHT) {
            vtext_move_cursor_right(editor);
            vtext_display(editor);
        } else if (is_key_available()) {
            char key = get_key_from_buffer();
            vtext_handle_key(editor, key);
            vtext_display(editor);
        }
        
        asm volatile("hlt");
    }
}

void vtext_display(vtext_editor *editor) {
    clearscr();
    
    int display_lines = 25 - VTEXT_STATUS_HEIGHT;
    
    for (int i = 0; i < display_lines && (editor->top_line + i) < editor->line_count; i++) {
        gotoxy(0, i);
        printf("%s", editor->lines[editor->top_line + i]);
    }
    
    gotoxy(0, 23);
    printf("File: %s %s | F12=Save F4=Exit | Line %d Col %d", 
           editor->filename, 
           editor->modified ? "(modified)" : "",
           editor->cursor_line + 1, 
           editor->cursor_col + 1);
    
    gotoxy(0, 24);
    printf("VText Editor - Simple text editor for VibexOS");
    
    int display_cursor_line = editor->cursor_line - editor->top_line;
    if (display_cursor_line >= 0 && display_cursor_line < display_lines) {
        update_hardware_cursor(editor->cursor_col, display_cursor_line);
    }
}

void vtext_handle_key(vtext_editor *editor, char key) {
    if (key == '\n') {
        vtext_new_line(editor);
    } else if (key == '\b') {
        vtext_delete_char(editor);
    } else if (key >= 32 && key <= 126) {
        vtext_insert_char(editor, key);
    }
}

void vtext_insert_char(vtext_editor *editor, char c) {
    char *line = editor->lines[editor->cursor_line];
    int line_len = strlen(line);
    
    if (line_len < VTEXT_MAX_LINE_LENGTH - 1) {
        for (int i = line_len; i > editor->cursor_col; i--) {
            line[i] = line[i - 1];
        }
        line[editor->cursor_col] = c;
        line[line_len + 1] = '\0';
        editor->cursor_col++;
        editor->modified = 1;
    }
}

void vtext_delete_char(vtext_editor *editor) {
    if (editor->cursor_col > 0) {
        char *line = editor->lines[editor->cursor_line];
        int line_len = strlen(line);
        
        for (int i = editor->cursor_col - 1; i < line_len - 1; i++) {
            line[i] = line[i + 1];
        }
        line[line_len - 1] = '\0';
        editor->cursor_col--;
        editor->modified = 1;
    } else if (editor->cursor_line > 0) {
        char *current_line = editor->lines[editor->cursor_line];
        char *previous_line = editor->lines[editor->cursor_line - 1];
        int prev_len = strlen(previous_line);
        
        if (prev_len + strlen(current_line) < VTEXT_MAX_LINE_LENGTH - 1) {
            strcpy(previous_line + prev_len, current_line);
            
            for (int i = editor->cursor_line; i < editor->line_count - 1; i++) {
                strcpy(editor->lines[i], editor->lines[i + 1]);
            }
            
            editor->line_count--;
            editor->cursor_line--;
            editor->cursor_col = prev_len;
            editor->modified = 1;
        }
    }
}

void vtext_new_line(vtext_editor *editor) {
    if (editor->line_count < VTEXT_MAX_LINES) {
        char *current_line = editor->lines[editor->cursor_line];
        
        for (int i = editor->line_count; i > editor->cursor_line + 1; i--) {
            strcpy(editor->lines[i], editor->lines[i - 1]);
        }
        
        strcpy(editor->lines[editor->cursor_line + 1], current_line + editor->cursor_col);
        current_line[editor->cursor_col] = '\0';
        
        editor->line_count++;
        editor->cursor_line++;
        editor->cursor_col = 0;
        editor->modified = 1;
        
        if (editor->cursor_line - editor->top_line >= 21) {
            editor->top_line++;
        }
    }
}

void vtext_save_file(vtext_editor *editor) {
    char content[RAMFS_MAX_FILE_SIZE];
    int pos = 0;
    
    for (int i = 0; i < editor->line_count && pos < RAMFS_MAX_FILE_SIZE - 2; i++) {
        int line_len = strlen(editor->lines[i]);
        
        for (int j = 0; j < line_len && pos < RAMFS_MAX_FILE_SIZE - 2; j++) {
            content[pos++] = editor->lines[i][j];
        }
        
        if (i < editor->line_count - 1 && pos < RAMFS_MAX_FILE_SIZE - 2) {
            content[pos++] = '\n';
        }
    }
    
    content[pos] = '\0';
    
    int result = ramfs_write_file(editor->filename, content);
    if (result == 0) {
        editor->modified = 0;
    }
}

void vtext_load_file(vtext_editor *editor) {
    struct ramfs_file *file = ramfs_find_file(editor->filename);
    if (file) {
        int line = 0;
        int col = 0;
        
        for (int i = 0; i < file->size && line < VTEXT_MAX_LINES; i++) {
            if (file->content[i] == '\n') {
                editor->lines[line][col] = '\0';
                line++;
                col = 0;
            } else if (col < VTEXT_MAX_LINE_LENGTH - 1) {
                editor->lines[line][col] = file->content[i];
                col++;
            }
        }
        
        if (line < VTEXT_MAX_LINES) {
            editor->lines[line][col] = '\0';
            editor->line_count = line + 1;
        }
    }
}

int vtext_get_special_key(void) {
    int key = get_last_function_key();
    if (key != 0) {
        clear_function_key();
        return key;
    }
    return 0;
}

int vtext_get_arrow_key(void) {
    int key = get_last_arrow_key();
    if (key != 0) {
        clear_arrow_key();
        return key;
    }
    return 0;
}

void vtext_move_cursor_up(vtext_editor *editor) {
    if (editor->cursor_line > 0) {
        editor->cursor_line--;
        int line_len = strlen(editor->lines[editor->cursor_line]);
        if (editor->cursor_col > line_len) {
            editor->cursor_col = line_len;
        }
        
        if (editor->cursor_line < editor->top_line) {
            editor->top_line = editor->cursor_line;
        }
    }
}

void vtext_move_cursor_down(vtext_editor *editor) {
    if (editor->cursor_line < editor->line_count - 1) {
        editor->cursor_line++;
        int line_len = strlen(editor->lines[editor->cursor_line]);
        if (editor->cursor_col > line_len) {
            editor->cursor_col = line_len;
        }
        
        if (editor->cursor_line - editor->top_line >= 21) {
            editor->top_line++;
        }
    }
}

void vtext_move_cursor_left(vtext_editor *editor) {
    if (editor->cursor_col > 0) {
        editor->cursor_col--;
    } else if (editor->cursor_line > 0) {
        editor->cursor_line--;
        editor->cursor_col = strlen(editor->lines[editor->cursor_line]);
        
        if (editor->cursor_line < editor->top_line) {
            editor->top_line = editor->cursor_line;
        }
    }
}

void vtext_move_cursor_right(vtext_editor *editor) {
    int line_len = strlen(editor->lines[editor->cursor_line]);
    if (editor->cursor_col < line_len) {
        editor->cursor_col++;
    } else if (editor->cursor_line < editor->line_count - 1) {
        editor->cursor_line++;
        editor->cursor_col = 0;
        
        if (editor->cursor_line - editor->top_line >= 21) {
            editor->top_line++;
        }
    }
}