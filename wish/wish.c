#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "wish.h"

int main(int arc, char *argv[]){
	//char* path = "/usr/bin/";
	char* usr_input;
	ssize_t read;
	size_t len;

	int arg_count;
	char** cmd_args;
	char* arg = NULL;
	const char space[1] = " ";
	
	struct command* c;
	int res;
	
	while (1) {
		// Reset everything for next loop.
		usr_input = NULL;
		read = 0;
		len = 0;
		res = 0;

		printf("wish> ");
		read = getline(&usr_input, &len, stdin);
		if (read == -1) {
			fprintf(stderr, "failed to get user input");
			return 1;
		}
		
		arg_count = 0;
		cmd_args = malloc(MAX_ARG_LEN*sizeof(char*));
		arg = NULL;
		// we don't have to pass the string again after this. 
		arg = strtok(usr_input, space);
		// if we don't find a token we just ignore command.
		if (arg == NULL){
			continue;
		}
		cmd_args[arg_count] = arg;
		arg_count++;

		while ((arg = strtok(NULL, space)) != NULL) {
			cmd_args[arg_count] = arg;
			if (arg_count > MAX_ARG_LEN){
				break;
			}	
			arg_count++;
		}
		
		cmd_args = realloc(cmd_args, (arg_count*sizeof(char*)+1));
		if (cmd_args == NULL) {
			fprintf(stderr, "failed to realloc user args");
			return 1;
		}
	
		cmd_args[arg_count+1] = "\0";
		
		c = malloc(sizeof(struct command));
		if (c == NULL) {
			fprintf(stderr, "failed to malloc command c");
			return 1;
		}	
		
		c->args = cmd_args;
		c->arg_count = arg_count;

		res = handle_cmd(c);
		if (res != 0) {
			fprintf(stderr, "failed to handle command");
			return 1;
		}	
		
		// Cleanup memory
		free(c->args[0]);
		free(cmd_args);
		free(c);
	}
}

int handle_cmd(struct command* cmd){
	int status;
	pid_t pid, w;

	int strip = strip_newline(cmd);	
	if (strip != 0) {
		return 1;	
	}
	
	pid = fork();
	if (pid == -1){
		return 2;
	}
	else if (pid == 0){
		// handle command
		execvp(cmd->args[0], cmd->args);
		return 3;
	}
	else {
		if ((w = wait(&status)) < 0) {
			return 4;
		}
	}

	return 0;
}

int strip_newline(struct command* cmd) {
	char *newline = "\n";
	char *null_char = "\0";

	for (int i = 0; i < cmd->arg_count; i++) {	
		char* line = *(cmd->args + i);
		int match = strcmp( (line+strlen(line)-1), newline );
		if (match == 0) {
			*(line+strlen(line)-1) = *null_char;
		}
	}

	return 0;
}
