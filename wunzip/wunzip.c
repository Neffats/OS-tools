#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *fp;
	char current_char;
	int count;
	
	if (argc == 1) {
		printf("wunzip: file1 [file2 ...]\n");
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("wunzip: cannot open file\n");
			return 1;
		}

		while(!(feof(fp))){	
			fread(&count, sizeof(int), 1, fp);
			if (feof(fp)) {
				break;
			}

			fread(&current_char, sizeof(char), 1, fp);
			if (feof(fp)) {
				break;
			}

			for (int j = 0; j < count; j++) {
				printf("%c", current_char);
			}
		}
	}

	return 0;
}
