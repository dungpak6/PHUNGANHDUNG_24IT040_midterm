#include "ls.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const Options *sorting;
static struct timespec entry_time(const Entry *e) {
    struct timespec t;
    switch (sorting->time_mode) {
    case TIME_ACCESS: t = e->st.st_atim; break;
    case TIME_CHANGE: t = e->st.st_ctim; break;
    default: t = e->st.st_mtim; break;
    }
    return t;
}
int entry_compare(const void *va, const void *vb) {
    const Entry *a = va, *b = vb;
    int k = 0;
    if (sorting->sort == SORT_SIZE) {
        if (a->st.st_size > b->st.st_size) k = -1;
        else if (a->st.st_size < b->st.st_size) k = 1;
    } else if (sorting->sort == SORT_TIME) {
        struct timespec ta = entry_time(a), tb = entry_time(b);
        if (ta.tv_sec > tb.tv_sec) k = -1;
        else if (ta.tv_sec < tb.tv_sec) k = 1;
        else if (ta.tv_nsec > tb.tv_nsec) k = -1;
        else if (ta.tv_nsec < tb.tv_nsec) k = 1;
    }
    if (!k) k = strcmp(a->name, b->name);
    return sorting->reverse ? -k : k;
}
void sort_entries(EntryList *list, const Options *o) {
    if (o->sort == SORT_NONE || list->count < 2) return;
    sorting = o;
    qsort(list->items, list->count, sizeof(*list->items), entry_compare);
}
