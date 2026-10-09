#include "ls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#ifndef S_ISVTX
#define S_ISVTX 01000
#endif
#ifndef __NetBSD__
#include <sys/sysmacros.h>
#endif

static unsigned long long block_unit(const Options *o) {
    const char *s;
    char *end;
    unsigned long long n;
    if (o->size_mode == SIZE_HUMAN || o->size_mode == SIZE_KILO) return 1024;
    s = getenv("BLOCKSIZE");
    if (!s || !*s) return 512;
    n = strtoull(s, &end, 10);
    if (!n) return 512;
    if (*end == 'k' || *end == 'K') { if (n > UINT64_MAX/1024) return 512; n *= 1024; ++end; }
    else if (*end == 'm' || *end == 'M') { if (n > UINT64_MAX/(1024*1024)) return 512; n *= 1024*1024; ++end; }
    else if (*end == 'g' || *end == 'G') { if (n > UINT64_MAX/(1024ULL*1024*1024)) return 512; n *= 1024ULL*1024*1024; ++end; }
    if (*end) return 512;
    return n;
}
unsigned long long used_blocks(const struct stat *st, const Options *o) {
    /* st_blocks counts 512-byte blocks on NetBSD and POSIX-like systems. */
    unsigned long long n = st->st_blocks > 0 ? (unsigned long long)st->st_blocks : 0;
    unsigned long long unit = block_unit(o);
    if (n > UINT64_MAX/512) return UINT64_MAX/unit;
    n *= 512;
    return n / unit + (n % unit != 0);
}
static void human_size(unsigned long long n, char *buf, size_t len) {
    static const char *unit[] = {"B", "K", "M", "G", "T", "P", "E"};
    unsigned i = 0;
    double v = (double)n;
    while (v >= 1024 && i < 6) { v /= 1024; ++i; }
    if (i && v < 10) snprintf(buf, len, "%.1f%s", v, unit[i]);
    else snprintf(buf, len, "%.0f%s", v, unit[i]);
}
static void print_name(const char *s, const Options *o) {
    const unsigned char *p = (const unsigned char *)s;
    while (*p) {
        if (o->quote && !isprint(*p)) putchar('?');
        else putchar(*p);
        ++p;
    }
}
static char file_type(mode_t m) {
    if (S_ISDIR(m)) return 'd';
    if (S_ISLNK(m)) return 'l';
    if (S_ISCHR(m)) return 'c';
    if (S_ISBLK(m)) return 'b';
    if (S_ISFIFO(m)) return 'p';
    if (S_ISSOCK(m)) return 's';
#ifdef __NetBSD__
    if (S_ISWHT(m)) return 'w';
#endif
    return '-';
}
static char marker(mode_t m) {
    if (S_ISDIR(m)) return '/';
    if (S_ISLNK(m)) return '@';
    if (S_ISSOCK(m)) return '=';
    if (S_ISFIFO(m)) return '|';
#ifdef __NetBSD__
    if (S_ISWHT(m)) return '%';
#endif
    if (S_ISREG(m) && (m & (S_IXUSR|S_IXGRP|S_IXOTH))) return '*';
    return '\0';
}
static void print_mode(mode_t m) {
    char s[] = "----------";
    const mode_t bits[] = {S_IRUSR,S_IWUSR,S_IXUSR,S_IRGRP,S_IWGRP,S_IXGRP,S_IROTH,S_IWOTH,S_IXOTH};
    const char chars[] = "rwxrwxrwx";
    int i;
    s[0] = file_type(m);
    for (i = 0; i < 9; ++i) if (m & bits[i]) s[i+1] = chars[i];
    if (m & S_ISUID) s[3] = m & S_IXUSR ? 's' : 'S';
    if (m & S_ISGID) s[6] = m & S_IXGRP ? 's' : 'S';
    if (m & S_ISVTX) s[9] = m & S_IXOTH ? 't' : 'T';
    fputs(s, stdout);
}
static time_t chosen_time(const Entry *e, const Options *o) {
    if (o->time_mode == TIME_ACCESS) return e->st.st_atime;
    if (o->time_mode == TIME_CHANGE) return e->st.st_ctime;
    return e->st.st_mtime;
}
static void print_long(const Entry *e, const Options *o) {
    struct passwd *pw = NULL;
    struct group *gr = NULL;
    char owner[32], group[32], date[64], size[64];
    time_t t = chosen_time(e, o), now = time(NULL);
    struct tm tmval;
    int recent = (t <= now + 3600 && t >= now - (time_t)(365LL*24*3600/2));
    print_mode(e->st.st_mode);
    printf(" %3ju ", (uintmax_t)e->st.st_nlink);
    if (!o->numeric) { pw = getpwuid(e->st.st_uid); gr = getgrgid(e->st.st_gid); }
    if (!pw) snprintf(owner, sizeof(owner), "%ju", (uintmax_t)e->st.st_uid);
    if (!gr) snprintf(group, sizeof(group), "%ju", (uintmax_t)e->st.st_gid);
    printf("%-8s %-8s ", pw ? pw->pw_name : owner, gr ? gr->gr_name : group);
    if (S_ISCHR(e->st.st_mode) || S_ISBLK(e->st.st_mode)) {
        snprintf(size, sizeof(size), "%u, %u", (unsigned)major(e->st.st_rdev), (unsigned)minor(e->st.st_rdev));
    } else if (o->size_mode == SIZE_HUMAN && e->st.st_size >= 0) {
        human_size((unsigned long long)e->st.st_size, size, sizeof(size));
    } else snprintf(size, sizeof(size), "%jd", (intmax_t)e->st.st_size);
    printf("%8s ", size);
    if (localtime_r(&t, &tmval))
        strftime(date, sizeof(date), recent ? "%b %e %H:%M" : "%b %e  %Y", &tmval);
    else snprintf(date, sizeof(date), "??? ?? ??:??");
    printf("%s ", date);
}
static void print_link_target(const Entry *e, const Options *o) {
    size_t cap = 128;
    char *buf = NULL;
    for (;;) {
        ssize_t n;
        char *p;
        if (cap > 1024*1024*16) break;
        p = realloc(buf, cap);
        if (!p) break;
        buf = p;
        n = readlink(e->path, buf, cap-1);
        if (n < 0) break;
        if ((size_t)n < cap-1) {
            buf[n] = '\0';
            fputs(" -> ", stdout);
            print_name(buf, o);
            break;
        }
        cap *= 2;
    }
    free(buf);
}
void print_entry(const Entry *e, const Options *o) {
    if (o->inode) printf("%ju ", (uintmax_t)e->st.st_ino);
    if (o->blocks) {
        unsigned long long b = used_blocks(&e->st, o);
        if (o->size_mode == SIZE_HUMAN) {
            char size[40];
            human_size(b > UINT64_MAX/1024 ? UINT64_MAX : b * 1024, size, sizeof(size));
            printf("%s ", size);
        } else printf("%llu ", b);
    }
    if (o->longfmt) print_long(e, o);
    print_name(e->name, o);
    if (o->classify) { char ch = marker(e->st.st_mode); if (ch) putchar(ch); }
    if (o->longfmt && S_ISLNK(e->st.st_mode)) print_link_target(e, o);
    putchar('\n');
}
void print_entries(const EntryList *list, const Options *o, int is_directory) {
    size_t i;
    if (is_directory && (o->longfmt || (o->blocks && isatty(STDOUT_FILENO)))) {
        unsigned long long total = 0;
        for (i = 0; i < list->count; ++i) {
            unsigned long long blocks = (unsigned long long)(list->items[i].st.st_blocks > 0 ? list->items[i].st.st_blocks : 0);
            if (UINT64_MAX - total < blocks) { total = UINT64_MAX; break; }
            total += blocks;
        }
        if (o->size_mode == SIZE_HUMAN) {
            char b[40]; human_size(total > UINT64_MAX/512 ? UINT64_MAX : total*512, b, sizeof(b));
            printf("total %s\n", b);
        } else {
            unsigned long long unit = block_unit(o);
            unsigned long long blocks = total > UINT64_MAX/512 ? UINT64_MAX : total*512;
            printf("total %llu\n", blocks/unit + (blocks%unit != 0));
        }
    }
    for (i = 0; i < list->count; ++i) print_entry(&list->items[i], o);
}
