#include "ls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

static int visible(const char *s, const Options *o) {
    if (o->all) return 1;
    if (o->almost_all) return strcmp(s, ".") != 0 && strcmp(s, "..") != 0;
    return s[0] != '.';
}
static int read_directory(const char *path, const Options *o, EntryList *list, int *opened) {
    DIR *dir = opendir(path);
    struct dirent *de;
    int failed = 0;
    if (!dir) { fprintf(stderr, "myls: %s: %s\n", path, strerror(errno)); return 1; }
    *opened = 1;
    errno = 0;
    while ((de = readdir(dir)) != NULL) {
        char *full;
        struct stat st;
        if (!visible(de->d_name, o)) { errno = 0; continue; }
        full = path_join(path, de->d_name);
        if (!full) { perror("myls: allocation"); failed = 1; break; }
        if (lstat(full, &st) < 0) {
            fprintf(stderr, "myls: %s: %s\n", full, strerror(errno));
            failed = 1;
        } else if (entry_list_add(list, de->d_name, full, &st, 0) < 0) {
            perror("myls: allocation"); free(full); failed = 1; break;
        }
        free(full);
        errno = 0;
    }
    if (errno) { fprintf(stderr, "myls: reading %s: %s\n", path, strerror(errno)); failed = 1; }
    if (closedir(dir) != 0) { perror("myls: closedir"); failed = 1; }
    return failed;
}
int list_directory(const char *path, const Options *o, int header, int *printed) {
    EntryList list;
    size_t i;
    int failed, opened = 0;
    entry_list_init(&list);
    failed = read_directory(path, o, &list, &opened);
    if (!opened) { entry_list_free(&list); return 1; }
    if (header) {
        if (*printed) putchar('\n');
        printf("%s:\n", path);
    } else if (*printed) putchar('\n');
    *printed = 1;
    sort_entries(&list, o);
    print_entries(&list, o, 1);
    if (o->recursive) {
        for (i = 0; i < list.count; ++i) {
            const Entry *e = &list.items[i];
            if (!S_ISDIR(e->st.st_mode)) continue;
            if (strcmp(e->name, ".") == 0 || strcmp(e->name, "..") == 0) continue;
            if (list_directory(e->path, o, 1, printed)) failed = 1;
        }
    }
    entry_list_free(&list);
    return failed;
}
int list_operands(int argc, char **argv, int first, const Options *o) {
    EntryList files, dirs;
    size_t i;
    int errors = 0, printed = 0;
    int count = argc - first;
    entry_list_init(&files); entry_list_init(&dirs);
    if (!count) {
        errors = list_directory(".", o, o->recursive, &printed);
        return errors;
    }
    for (i = (size_t)first; i < (size_t)argc; ++i) {
        struct stat st;
        int isdir;
        if (lstat(argv[i], &st) < 0) {
            fprintf(stderr, "myls: %s: %s\n", argv[i], strerror(errno));
            errors = 1; continue;
        }
        /* Normally follow symlinks given as command-line operands, except -d. */
        if (!o->directory && S_ISLNK(st.st_mode)) {
            struct stat followed;
            if (stat(argv[i], &followed) == 0 && S_ISDIR(followed.st_mode)) st = followed;
        }
        isdir = S_ISDIR(st.st_mode) && !o->directory;
        if (entry_list_add(isdir ? &dirs : &files, argv[i], argv[i], &st, isdir) < 0) {
            perror("myls: allocation"); errors = 1; goto cleanup;
        }
    }
    sort_entries(&files, o);
    sort_entries(&dirs, o);
    print_entries(&files, o, 0);
    if (files.count) printed = 1;
    for (i = 0; i < dirs.count; ++i) {
        if (list_directory(dirs.items[i].path, o,
                count > 1 || o->recursive, &printed)) errors = 1;
    }
cleanup:
    entry_list_free(&files);
    entry_list_free(&dirs);
    return errors;
}
