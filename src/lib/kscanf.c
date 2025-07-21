#include "../include/kscanf.h"
#include "../include/keyboard.h"

static char input_buffer[256];
static int input_pos = 0;

static int str_to_int(const char* str) {
    int result = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    } else if (str[0] == '+') {
        i = 1;
    }
    
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return sign * result;
}

static int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static void read_input_line(void) {
    input_pos = 0;
    
    while (1) {
        while (!is_key_available()) {
            asm volatile("hlt");
        }
        
        char key = get_key_from_buffer();
        
        if (key == '\n') {
            input_buffer[input_pos] = '\0';
            break;
        } else if (key == '\b') {
            if (input_pos > 0) {
                input_pos--;
            }
        } else if (key >= 32 && key <= 126) {
            if (input_pos < 255) {
                input_buffer[input_pos++] = key;
            }
        }
    }
}

static void skip_whitespace(const char* input, int* pos) {
    while (input[*pos] && is_whitespace(input[*pos])) {
        (*pos)++;
    }
}

static int parse_word(const char* input, int* pos, char* dest, int max_len) {
    int len = 0;
    
    skip_whitespace(input, pos);
    
    while (input[*pos] && !is_whitespace(input[*pos]) && len < max_len - 1) {
        dest[len++] = input[(*pos)++];
    }
    
    dest[len] = '\0';
    return len;
}

static int parse_int(const char* input, int* pos, int* dest) {
    char temp_buffer[32];
    int len = parse_word(input, pos, temp_buffer, 32);
    
    if (len == 0) {
        return 0;
    }
    
    *dest = str_to_int(temp_buffer);
    return 1;
}

void kscanf(const char* format, ...) {
    read_input_line();
    
    int input_pos = 0;
    int format_pos = 0;
    
    char** args = (char**)(&format + 1);
    int arg_index = 0;
    
    while (format[format_pos]) {
        if (format[format_pos] == '%') {
            format_pos++;
            
            if (format[format_pos] == 's') {
                char* str_dest = (char*)args[arg_index];
                parse_word(input_buffer, &input_pos, str_dest, 32);
                arg_index++;
            } else if (format[format_pos] == 'd') {
                int* int_dest = (int*)args[arg_index];
                parse_int(input_buffer, &input_pos, int_dest);
                arg_index++;
            }
        }
        format_pos++;
    }
}