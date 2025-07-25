#ifndef VTEXT_H
#define VTEXT_H

#define VTEXT_MAX_LINES 200
#define VTEXT_MAX_LINE_LENGTH 80
#define VTEXT_STATUS_HEIGHT 2

typedef struct {
    char lines[VTEXT_MAX_LINES][VTEXT_MAX_LINE_LENGTH];
    int line_count;
    int cursor_line;
    int cursor_col;
    int top_line;
    char filename[32];
    int modified;
} vtext_editor;

void vtext_init(vtext_editor *editor, const char *filename);
void vtext_run(vtext_editor *editor);
void vtext_display(vtext_editor *editor);
void vtext_handle_key(vtext_editor *editor, char key);
void vtext_insert_char(vtext_editor *editor, char c);
void vtext_delete_char(vtext_editor *editor);
void vtext_new_line(vtext_editor *editor);
void vtext_save_file(vtext_editor *editor);
void vtext_load_file(vtext_editor *editor);
int vtext_get_special_key(void);
int vtext_get_arrow_key(void);
void vtext_move_cursor_up(vtext_editor *editor);
void vtext_move_cursor_down(vtext_editor *editor);
void vtext_move_cursor_left(vtext_editor *editor);
void vtext_move_cursor_right(vtext_editor *editor);

#define SPECIAL_KEY_F4  0x3E
#define SPECIAL_KEY_F12 0x58
#define SPECIAL_KEY_UP    0x48
#define SPECIAL_KEY_DOWN  0x50
#define SPECIAL_KEY_LEFT  0x4B
#define SPECIAL_KEY_RIGHT 0x4D

#endif