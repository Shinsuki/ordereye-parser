#pragma once

#include <stdio.h>

typedef struct {
	FILE *file;               // File pointer to open and read the file
	char *options;            // Stores the options passed via command line
	char (*arguments)[100];   // Array to store arguments passed to the program
	int *arguments_count;     // Pointer to store the number of arguments
}shell_info;

typedef struct {
	bool e_check; // Flag to check if the -e option is set
	bool l_check; // Flag to check if the -l option is set
	bool s_check; // Flag to check if the -s option is set
	bool p_check; // Flag to check if the -p option is set
	bool v_check; // Flag to check if the -v option is set
}bool_flag;

void alloccheck (void *pointer);
void help(char **arguments);
void processargs (int argc, char **argv, shell_info *shell_args);
bool check_arg(char *arg);
bool_flag checkoptions (char *options, bool_flag options_bool);
