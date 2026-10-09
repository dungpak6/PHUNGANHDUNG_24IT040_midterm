#include "ls.h"
#include <locale.h>
#include <stdio.h>
int main(int argc, char **argv) {
    Options options;
    int first;
    (void)setlocale(LC_ALL, "");
    options_init(&options);
    if (options_parse(argc, argv, &options, &first)) return 1;
    int status = list_operands(argc, argv, first, &options);
    if (fflush(stdout) == EOF || ferror(stdout)) { perror("myls: stdout"); return 1; }
    return status ? 1 : 0;
}
