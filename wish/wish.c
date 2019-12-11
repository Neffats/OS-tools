#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARG_LEN 100

int main(int arc, char *argv[]){
	char* usr_input = NULL;
	ssize_t read;
	size_t len = 0;

	//char* path = "/usr/bin/";

	while (1) {
		printf("wish> ");
		read = getline(&usr_input, &len, stdin);
		if (read == -1) {
			fprintf(stderr, "failed to get user input");
			return 1;
		}	
		
		int arg_count = 0;
		char** cmd_args = malloc(MAX_ARG_LEN*sizeof(char*));
		char* arg = NULL;
		const char space[2] = " ";
		while ((arg = strtok(usr_input, space)) != NULL) {
			cmd_args[arg_count] = arg;
			if (arg_count > MAX_ARG_LEN){
				break;
			}	
			arg_count++;
		}
		printf("%ld\n", sizeof(cmd_args));
		cmd_args = realloc(cmd_args, arg_count*sizeof(char*));
		
		if (cmd_args == NULL) {
			fprintf(stderr, "failed to realloc user args");
			return 1;
		}
		printf("%s\n", cmd_args[0]);
	}
}
