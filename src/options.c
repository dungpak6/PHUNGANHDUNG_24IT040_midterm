#include "ls.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void options_init(Options *o) {
    memset(o, 0, sizeof(*o));
    o->quote = isatty(STDOUT_FILENO) ? 1 : 0;
#ifdef __NetBSD__
    /* NetBSD ls enables -A by default for the super-user. */
    if (geteuid() == 0) o->almost_all = 1;
#endif
    o->sort = SORT_NAME;
    o->time_mode = TIME_MODIFY;
    o->size_mode = SIZE_DEFAULT;
}
int options_parse(int argc, char **argv, Options *o, int *first) {
    int ch;
    opterr = 0;
    while ((ch = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (ch) {
        case 'A': o->almost_all = 1; o->all = 0; break;
        case 'a': o->all = 1; o->almost_all = 0; break;
        case 'c': o->time_mode = TIME_CHANGE; break;
        case 'd': o->directory = 1; o->recursive = 0; break;
        case 'F': o->classify = 1; break;
        case 'f': o->sort = SORT_NONE; break;
        case 'h': o->size_mode = SIZE_HUMAN; break;
        case 'i': o->inode = 1; break;
        case 'k': o->size_mode = SIZE_KILO; break;
        case 'l': o->longfmt = 1; o->numeric = 0; break;
        case 'n': o->longfmt = 1; o->numeric = 1; break;
        case 'q': o->quote = 1; break;
        case 'R': o->recursive = 1; o->directory = 0; break;
        case 'r': o->reverse = 1; break;
        case 'S': o->sort = SORT_SIZE; break;
        case 's': o->blocks = 1; break;
        case 't': o->sort = SORT_TIME; break;
        case 'u': o->time_mode = TIME_ACCESS; break;
        case 'w': o->quote = 0; break;
        default:
            if (optopt) fprintf(stderr, "myls: unknown option -- %c\n", optopt);
            else fprintf(stderr, "myls: invalid arguments\n");
            fprintf(stderr, "usage: myls [-AacdFfhiklnqRrSstuw] [file ...]\n");
            return -1;
        }
    }
    *first = optind;
    return 0;
}
