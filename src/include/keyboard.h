#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_BUFFER_SIZE 256

void keyboard_init(void);
void keyboard_handler(void);
char get_key_from_buffer(void);
int is_key_available(void);
int get_last_function_key(void);
void clear_function_key(void);

#endif