#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "wish.h"

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
		// We only need to free args[0] since strtok() doesn't allocate any extra memory.
		free_command(c);
	}
}


int batch_mode(char* batch_file) {
	// Check that file exists
	if ( access(batch_file, F_OK) != 0 ) {
		return 1;
	}

	FILE *fp = fopen(batch_file, "r");

	ssize_t read;
	size_t len = 0;
	char *line = NULL;
	// Wh
	int line_count = 1;
	struct command** commands;
	struct command* cmd = NULL;
	
	commands = malloc(sizeof(struct command*)); 
	if (commands == NULL) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	}
	
	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("reading line: %d\n", line_count);
		//printf("line: %s\n", line);
		cmd = format_command(line);
		if (cmd == NULL) {
			return 2;
		}
		
		commands[line_count-1] = cmd;
		line_count++;
		// Is this too expensive? 
		// Might be better to allocate big buffer to start and resize after the loop?
		commands = realloc(commands, line_count * sizeof(struct command*));
		if (commands == NULL) {
			fprintf(stderr, "malloc failed\n");
			return 3;
		}
		
		// we need to reset the pointer each time so that getline allocates a new buffer for 
		// each line.
		line = NULL; 
	}
	
	fclose(fp);

	//printf("handling commands: %d\n", line_count);
	for (int i = 0; i < line_count-1; i++) {
		int result = handle_cmd(commands[i]);
		//printf("result: %d\n", result);
		if (result != 0) {
			return 4;
		}
	}	
	
	for (int i = 0; i < line_count-1; i++) {
		free_command(commands[i]);
	}	
	
	free(commands);

	return 0;
}

struct command* get_input() {
	char* usr_input = NULL;
	ssize_t read = 0;
	size_t len = 0;

	struct command* c;
	
	read = getline(&usr_input, &len, stdin);
	if (read == -1) {
		fprintf(stderr, "failed to get user input\n");
		return NULL;
	}
	
	c = format_command(usr_input);

	return c;
}

struct command* format_command(char* cmd) {
	int arg_count = 0;
	char** cmd_args;
	char* arg = NULL;
	const char space[1] = " ";
	struct command* c;
	
	cmd_args = malloc(MAX_ARG_LEN*sizeof(char*));
	arg = NULL;
	// we don't have to pass the string again after this. 
	arg = strtok(cmd, space);
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

/*
bool is_builtin(struct command *cmd) {
	 * Built-in commands:
	 *  - cd
	 *  - exit
	
	return false;
}
*/

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
	if (pid < 0){
		return 2;
	}
	else if (pid == 0){
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

void free_command(struct command* c) {
	free(c->args[0]);
	free(c->args);
	free(c);
}

int hash(char* key) {
	int key_len = strlen(key);
	int hash = 0;
	
	for (int i = 0; i < key_len; i++) {
		hash = hash + ((ALPHABET_SIZE^((key_len-1)-(i+1))) * (int)key[i]);		
	}

	return hash;
}


