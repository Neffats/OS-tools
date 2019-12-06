#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define INIT_SZ 1

char** sort(char** list, int len);
void quicksort(char** arr,  int low, int high);
int partition(char** arr, int low, int high);
int compare_files(char* file1, char* file2);
void reverse(char** arr, int len);

int main(int argc,  char *argv[]){
	FILE *fp_in;
	FILE *fp_out;

	if (argc == 1) {
		fp_in = stdin;
		fp_out = stdout;
	}
	else if (argc == 2){
		fp_in = fopen(argv[1], "r");
		fp_out = stdout;
	}
	else if (argc == 3) {
		fp_in = fopen(argv[1], "r");
		fp_out = fopen(argv[2], "w");

		if (compare_files(argv[1], argv[2]) != 0) {
			fprintf(stderr, "reverse: input and output file must differ\n");
			return 1;
		}
	}
	else {
		fprintf(stderr, "usage: reverse <input> <output>\n");
		return 1;
	}
	if (fp_in == NULL) {
		fprintf(stderr, "reverse: cannot open file \'%s\'\n", argv[1]);
		return 1;	
	}
	if (fp_out == NULL) {
		fprintf(stderr, "reverse: cannot open file \'%s\'\n", argv[2]);
                return 1;
	}

	char** lines = NULL;
	lines = malloc(sizeof(char*)); 
	if (lines == NULL) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	}

	ssize_t read;
	size_t len = 0;
	char *line = NULL;
	int line_count = 1;

	while ((read = getline(&line, &len, fp_in)) != -1) {
		lines[line_count-1] = line;
		line_count++;
		lines = realloc(lines, line_count * sizeof(char*));
		if (lines == NULL) {
			fprintf(stderr, "malloc failed\n");
			return 1;
		}
		line = NULL; //we need to reset the pointer each time so that getline allocates a new buffer for each line.
	}
	
	//printf("%d\n", line_count);
	//quicksort(lines, 0, line_count-2);
	reverse(lines, line_count-1);

	for (int i = 0; i < line_count-1; i++) {
		fprintf(fp_out, "%s", lines[i]);
	}

	free(line);
	free(lines);
	
	return 0;
}

void reverse(char** arr, int len){
	char* temp[len];
	for (int i = len; i >= 0; i--) {
		temp[len-i] = arr[i-1];
	}
	for (int i = 0; i < len; i++) {
		arr[i] = temp[i];
	}
}

void quicksort(char** arr,  int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high);

		quicksort(arr, low, pi-1);
		quicksort(arr, pi+1, high);
	}

}

int partition(char** arr, int low, int high){
	char* pivot = arr[high];

	int i = (low-1);
	
	for (int j = low; j <= high-1; j++){
		char* curr = arr[j];
		if (curr[0] < pivot[0]) {
			i++;
			char* temp = arr[j];
			arr[j] = arr[i];
			arr[i] = temp;
		}
	}	
	char* temp = arr[i+1];
	arr[i+1] = arr[high];
	arr[high] = temp;
	return (i+1);
}



int compare_files(char* file1, char* file2){
	struct stat sb1;
	struct stat sb2;
		
	if (stat(file1, &sb1) == -1){
		fprintf(stderr, "reverse: cannot open file \'%s\'\n", file1);
		exit(1);
	}
	if (stat(file2, &sb2) == -1){
		fprintf(stderr, "reverse: cannot open file \'%s\'\n", file2);
		exit(1);
	}
	if (sb1.st_ino == sb2.st_ino) {
		return 1;
	}
	return 0;
}

