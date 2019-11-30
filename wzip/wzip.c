#include <stdio.h>


void write_entry(int count, int character) {
	fwrite(&count, sizeof(count), 1, stdout);
	char char_out = (char)character;
	fwrite(&char_out, sizeof(char_out), 1, stdout);
	
}

int main(int argc, char *argv[]){
	FILE *fp;
	int current_char;
	int last_char = 0;
	int current_count = 0;

	if (argc == 1) {
		printf("wzip: file1 [file2 ...]\n");
		return 1;
	}
	
	int loop_count = 0; 
	// we iterate over each file given in the parameters.
	for (int i = 1; i < argc; i++) {
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("wzip: cannot open file\n");
		        return 1;	
		}
		// used to handle first iteration.
		// we want to ignore the first character since we can't compare it with the previous character. 
		while ((current_char = fgetc(fp)) != EOF){
			if (loop_count == 0) {
				// if 1st char, just skip to next character.
				last_char = current_char;
				loop_count++;
				continue;
			}
			if (current_char == last_char) {
				current_count++;
			}
			else {
				current_count++; // we need to count the last character appearance.
				write_entry(current_count, last_char);
				current_count = 0;	
			}
			loop_count++;		
			last_char = current_char;
		}
		
		fclose(fp);
	}
	current_count++; // we need to count the last character appearance.
	write_entry(current_count, last_char);
	
	return 0;
}
