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
#define ALPHABET_SIZE 127

typedef enum {CMD_CD, CMD_EXIT, CMD_OTHER} command_type;  
				  
typedef struct command {
	char** args;
	int arg_count;
} command;

void free_command(struct command* c);

int interactive_mode(); 
int batch_mode(char* batch_file);
command_type identify_command(command *cmd);
int handle_builtin_cmd(command *cmd, command_type type);
int handle_other_cmd(command *cmd, command_type type);
int handle_cmd(command *cmd);
int strip_newline(command *cmd); 
command* get_input();
command* format_command(char* cmd);
int hash(char* key);

#endif
