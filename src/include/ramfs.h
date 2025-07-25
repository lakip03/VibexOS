#ifndef RAMFS_H
#define RAMFS_H

#define RAMFS_MAX_FILES 64
#define RAMFS_MAX_FILENAME_LEN 32
#define RAMFS_MAX_FILE_SIZE 1024

struct ramfs_file {
    char name[RAMFS_MAX_FILENAME_LEN];
    char content[RAMFS_MAX_FILE_SIZE];
    int size;
    int used;
};

void ramfs_init(void);
int ramfs_create_file(const char *name, const char *content);
struct ramfs_file *ramfs_find_file(const char *name);
int ramfs_list_files(void);
int ramfs_read_file(const char *name);
int ramfs_load_ext_files(void);
int ramfs_write_file(const char *name, const char *content);

#endif