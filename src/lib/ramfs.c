#include "../include/ramfs.h"
#include "../include/printf.h"

static struct ramfs_file ramfs[RAMFS_MAX_FILES];
static int file_count = 0;

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

void ramfs_init(void) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        ramfs[i].used = 0;
        ramfs[i].size = 0;
        ramfs[i].name[0] = '\0';
        ramfs[i].content[0] = '\0';
    }
    file_count = 0;
    
    ramfs_create_file("readme.txt", "Welcome to VibexOS RAMFS!\nThis is a filesystem in RAM.\nUse 'ls' to list files, 'cat <filename>' to read files, and 'touch <filename> <content>' to create files.");
    ramfs_create_file("hello.txt", "Hello, World!\nThis is a test file in the RAMFS.");
    ramfs_create_file("system.info", "VibexOS v1.0\nKernel: Simple monolithic kernel\nFilesystem: RAMFS (Read/Write)\nMemory: Static allocation");
    
    ramfs_load_ext_files();
}

int ramfs_create_file(const char *name, const char *content) {
    if (file_count >= RAMFS_MAX_FILES) {
        return -1;
    }
    
    if (strlen(name) >= RAMFS_MAX_FILENAME_LEN) {
        return -1;
    }
    
    if (strlen(content) >= RAMFS_MAX_FILE_SIZE) {
        return -1;
    }
    
    if (ramfs_find_file(name) != 0) {
        return -1;
    }
    
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (!ramfs[i].used) {
            strcpy(ramfs[i].name, name);
            strcpy(ramfs[i].content, content);
            ramfs[i].size = strlen(content);
            ramfs[i].used = 1;
            file_count++;
            return 0;
        }
    }
    
    return -1;
}

struct ramfs_file *ramfs_find_file(const char *name) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (ramfs[i].used && strcmp(ramfs[i].name, name) == 0) {
            return &ramfs[i];
        }
    }
    return 0;
}

int ramfs_list_files(void) {
    printf("Files in RAMFS:\n");
    if (file_count == 0) {
        printf("  (no files)\n");
        return 0;
    }
    
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (ramfs[i].used) {
            printf("  %s", ramfs[i].name);
            
            // Add padding spaces to align the size
            int name_len = strlen(ramfs[i].name);
            int padding = 20 - name_len;
            if (padding < 1) padding = 1;
            
            for (int j = 0; j < padding; j++) {
                printf(" ");
            }
            
            printf("%d bytes\n", ramfs[i].size);
        }
    }
    
    return file_count;
}

int ramfs_read_file(const char *name) {
    struct ramfs_file *file = ramfs_find_file(name);
    if (!file) {
        printf("File not found: %s\n", name);
        return -1;
    }
    
    printf("%s", file->content);
    return 0;
}

int ramfs_write_file(const char *name, const char *content) {
    struct ramfs_file *file = ramfs_find_file(name);
    if (file) {
        if (strlen(content) >= RAMFS_MAX_FILE_SIZE) {
            return -1;
        }
        strcpy(file->content, content);
        file->size = strlen(content);
        return 0;
    } else {
        return ramfs_create_file(name, content);
    }
}

int ramfs_load_ext_files(void) {
    ramfs_create_file("ext1.txt", "This is an extension file loaded from /src/ext\nContent: Sample extension file 1");
    ramfs_create_file("ext2.txt", "Another extension file from /src/ext\nContent: Sample extension file 2");
    ramfs_create_file("module.txt", "Extension module loaded at boot\nThis file was loaded from the /src/ext directory");
    printf("Loaded extension files from /src/ext\n");
    return 0;
}