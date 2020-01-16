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

int interactive_mode(); 
int handle_cmd(struct command* cmd);
int strip_newline(struct command* cmd); 
struct command* get_input();

#endif
