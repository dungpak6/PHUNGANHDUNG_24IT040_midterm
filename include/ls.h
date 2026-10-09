#ifndef MYLS_LS_H
#define MYLS_LS_H
#define _POSIX_C_SOURCE 200809L
#ifdef __NetBSD__
#ifndef _NETBSD_SOURCE
#define _NETBSD_SOURCE 1
#endif
#endif
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef enum { SORT_NAME, SORT_SIZE, SORT_TIME, SORT_NONE } SortMode;
typedef enum { TIME_MODIFY, TIME_ACCESS, TIME_CHANGE } TimeMode;
typedef enum { SIZE_DEFAULT, SIZE_KILO, SIZE_HUMAN } SizeMode;
typedef struct {
    int all, almost_all, directory, classify, inode, longfmt, numeric;
    int quote, recursive, reverse, blocks;
    SortMode sort;
    TimeMode time_mode;
    SizeMode size_mode;
} Options;

typedef struct {
    char *name;
    char *path;
    struct stat st;
    int stat_ok;
    int is_dir_operand;
} Entry;
typedef struct { Entry *items; size_t count, capacity; } EntryList;

void options_init(Options *o);
int options_parse(int argc, char **argv, Options *o, int *first);
void entry_list_init(EntryList *list);
int entry_list_add(EntryList *list, const char *name, const char *path,
                   const struct stat *st, int is_dir_operand);
void entry_list_free(EntryList *list);
char *path_join(const char *dir, const char *name);
int entry_compare(const void *va, const void *vb);
void sort_entries(EntryList *list, const Options *o);
int list_operands(int argc, char **argv, int first, const Options *o);
int list_directory(const char *path, const Options *o, int header, int *printed);
void print_entries(const EntryList *list, const Options *o, int is_directory);
void print_entry(const Entry *e, const Options *o);
unsigned long long used_blocks(const struct stat *st, const Options *o);
#endif
