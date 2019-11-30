#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
	    return 0;
    }
    
    for (int i = 1; i < argc; i++) { 
	    FILE *fp = fopen(argv[i], "r");
	    if (fp == NULL) {
		printf("wcat: cannot open file\n");
		return 1;
	    }

	    char buffer[1024];

	    while ( fgets(buffer, 60, fp) != NULL) {
		printf("%s", buffer);
	    }
	    fclose(fp);
    }
    return 0;
}
