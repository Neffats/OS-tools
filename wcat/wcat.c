#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("too many arguments supplied. want: 2 got: %d\n", argc);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("failed to open file: %s\n", argv[1]);
        return 1;
    }

    char buffer[1024];

    while ( fgets(buffer, 60, fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}