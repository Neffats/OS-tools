#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARG_LEN 100

struct command {
	char** args;
	int arg_count;
};

void free_command(struct command* c);

int interactive_mode(); 
int batch_mode(char* batch_file);
int handle_cmd(struct command* cmd);
int strip_newline(struct command* cmd); 
struct command* get_input();
struct command* format_command(char* cmd);

#endif
