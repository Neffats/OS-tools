#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "wish.h"

int main(int argc, char *argv[]){
	//char* path = "/usr/bin/";
	int result = 0;
	
	switch(argc) {
		// No arg given so run in interactive mode.
		case 1:
			result = interactive_mode();
		default:
			result = 1;
	}

	if (result != 0) {
		return 1;
	}
}

int interactive_mode() {
	int res;
	while (1) {
		// Reset everything for next loop.
		res = 0;

		printf("wish> ");
		struct command* c = get_input();
		if (c == NULL) {
			continue;
		}
		res = handle_cmd(c);
		if (res != 0) {
			fprintf(stderr, "failed to handle command\n");
			return 1;
		}	
		
		// Cleanup memory
		free(c->args[0]);
		free(c->args);
		free(c);
	}
}

struct command* get_input() {
	char* usr_input = NULL;
	ssize_t read = 0;
	size_t len = 0;

	int arg_count = 0;
	char** cmd_args;
	char* arg = NULL;
	const char space[1] = " ";
	
	struct command* c;
	
	read = getline(&usr_input, &len, stdin);
	if (read == -1) {
		fprintf(stderr, "failed to get user input\n");
		return NULL;
	}
	
	arg_count = 0;
	cmd_args = malloc(MAX_ARG_LEN*sizeof(char*));
	arg = NULL;
	// we don't have to pass the string again after this. 
	arg = strtok(usr_input, space);
	// if we don't find a token we just ignore command.
	if (arg == NULL){
		return NULL;
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
		fprintf(stderr, "failed to realloc user args\n");
		return NULL;
	}

	cmd_args[arg_count+1] = "\0";
	
	c = malloc(sizeof(struct command));
	if (c == NULL) {
		fprintf(stderr, "failed to malloc command c\n");
		return NULL;
	}	
	
	c->args = cmd_args;
	c->arg_count = arg_count;
	
	return c;
}

int handle_cmd(struct command* cmd){
	int status;
	pid_t pid, w;

	int strip = strip_newline(cmd);	
	char exit_cmd[] = "exit";

	if (strip != 0) {
		return 1;	
	}
	
	if (cmd->arg_count == 1) {
		if ((strcmp(cmd->args[0], exit_cmd)) == 0) {
			exit(0);
		}
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

