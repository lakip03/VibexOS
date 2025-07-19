#ifndef PRINTF_H
#define PRINTF_H

void printf(const char *format, ...);
void putchar(char c);
void print_char(char c);
void print_string(const char *str);
void print_int(int num);
void print_hex(unsigned int num);
int strlen(const char *str);
char *itoa(int value, char *buffer, int base);

#endif