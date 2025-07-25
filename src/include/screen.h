#ifndef SCREEN_H
#define SCREEN_H

void clearscr(void);
void gotoxy(int x, int y);
void set_cursor_position(int x, int y);
void update_hardware_cursor(int x, int y);

#endif