#ifndef PRINTF_H
#define PRINTF_H

#ifndef _STDARG_H
typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)
#endif

extern int cursor_x;
extern int cursor_y;

void printf(const char *format, ...);
void putchar(char c);
void print_char(char c);
void print_string(const char *str);
void print_int(int num);
void print_hex(unsigned int num);
int strlen(const char *str);
char *itoa(int value, char *buffer, int base);

#endif