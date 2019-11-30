#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(){
	printf("wgrep: searchterm [file ...]\n");
}

int main(int argc, char *argv[]) {
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	if (argc == 1) {
		print_usage();
		return 1;
	}
	
	if (argc == 2) {
		// If not file given then take input from stdin.
		fp = stdin;
	}
	else {
		// Open provided filename.
		fp = fopen(argv[2], "r");
	}	

	if (fp == NULL) {
		printf("wgrep: cannot open file\n");
		return 1;
	}
	
	while ((read = getline(&line, &len, fp)) != -1) {
		char *match = strstr(line, argv[1]);
		if (match != NULL) {
			printf("%s", line);
		}	
	}
		
	return 0;
}


