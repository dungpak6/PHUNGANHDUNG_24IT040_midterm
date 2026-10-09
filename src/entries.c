#include "ls.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

void entry_list_init(EntryList *list) { memset(list, 0, sizeof(*list)); }
void entry_list_free(EntryList *list) {
    size_t i;
    for (i = 0; i < list->count; ++i) {
        free(list->items[i].name);
        free(list->items[i].path);
    }
    free(list->items);
    entry_list_init(list);
}
int entry_list_add(EntryList *list, const char *name, const char *path,
                   const struct stat *st, int is_dir_operand) {
    Entry *e;
    if (list->count == list->capacity) {
        size_t n = list->capacity ? list->capacity * 2 : 32;
        Entry *p;
        if (n < list->capacity || n > SIZE_MAX / sizeof(*p)) { errno = ENOMEM; return -1; }
        p = realloc(list->items, n * sizeof(*p));
        if (!p) return -1;
        list->items = p; list->capacity = n;
    }
    e = &list->items[list->count];
    memset(e, 0, sizeof(*e));
    e->name = strdup(name);
    e->path = strdup(path);
    if (!e->name || !e->path) {
        free(e->name); free(e->path);
        memset(e, 0, sizeof(*e));
        return -1;
    }
    e->st = *st;
    e->stat_ok = 1;
    e->is_dir_operand = is_dir_operand;
    list->count++;
    return 0;
}
char *path_join(const char *dir, const char *name) {
    size_t a = strlen(dir), b = strlen(name);
    int slash = a != 0 && dir[a-1] != '/';
    char *p;
    if (a > SIZE_MAX - b - (size_t)slash - 1) { errno = ENOMEM; return NULL; }
    p = malloc(a + b + (size_t)slash + 1);
    if (!p) return NULL;
    memcpy(p, dir, a);
    if (slash) p[a++] = '/';
    memcpy(p + a, name, b + 1);
    return p;
}
