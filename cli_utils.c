#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "cli_utils.h"

void alloccheck (void *pointer)
{
	if (!pointer)
	{
		fprintf(stderr, "Memory Allocation Fail"); // Check for memory allocation failure
		exit(EXIT_FAILURE);
	}
}

void help(char **arguments)
{
	for (int i = 0; arguments[i]; i++) 
	{
		if(strcmp(arguments[i], "--help") == 0) 
		{
			printf("Usage: godeye [Options] [file]"); // Help message for usage
			exit(EXIT_SUCCESS);
		}
	}
}

void processargs (int argc, char **argv, shell_info *shell_args)
{
	bool file_found = false;
	int options_count = 0;
	for (int i = 1; argv[i]; i++) 
	{
		bool optget = false;
		if (argv[i][0] != '-' && !file_found) 
		{ // File argument found
			shell_args->file = fopen(argv[i], "r");
			if (!shell_args->file) 
			{
				fprintf(stderr, "File not found"); // Handle file not found
				exit(EXIT_FAILURE);
			}
			
			file_found = true;
			continue;
		} else if (argv[i][0] != '-' && file_found) 
		{ // Additional arguments after the file
			strcpy(shell_args->arguments[*shell_args->arguments_count], argv[i]);
			++*shell_args->arguments_count;
			continue;
		}

		for (int i2 = 0; argv[i][i2] != '\0'; i2++) 
		{
			if (i2 == 0 && argv[i][0] == '-') 
			{
				optget = true;
				++i2;

			} else if (argv[i][0] != '-') 
			{
				break;
			}
			if (optget) 
			{
				printf("%c", shell_args->options[i2]);
				shell_args->options[options_count] = argv[i][i2];
				++options_count;
			}
		}
	}
	shell_args->options[options_count] = '\0';

}

bool check_arg(char *arg)
{
	for (int i = 0; arg[i] != '\0'; i++) 
	{
		if (!isdigit(arg[i])) 
		{
			return false; // Argument is not a number
		}
	}
	return true; // Argument is a number
}

bool_flag checkoptions (char *options, bool_flag options_bool)
{
	for(int i = 0; options[i] != '\0'; i++)
	{
		if (options[i] == 'e') 
		{
			options_bool.e_check = true;
		}
		else if (options[i] == 'l') 
		{
			options_bool.l_check = true;
		}
		else if (options[i] == 's') 
		{
			options_bool.s_check = true;
		}
		else if (options[i] == 'p') 
		{
			options_bool.p_check = true;
		}
		else if (options[i] == 'v')
		{
			options_bool.v_check = true;
		}
	}

	// Ensure conflicting options aren't used together
	bool test_bool1 = options_bool.l_check && options_bool.p_check;
	bool test_bool2 = options_bool.p_check && options_bool.s_check;
	bool test_bool3 = options_bool.s_check && options_bool.l_check;
	bool test_bool4 = options_bool.v_check && !options_bool.v_check;
	if (test_bool1 || test_bool2 || test_bool3 || test_bool4) 
	{
		fprintf(stderr, "Options S, P and L can not be put together.\nTry 'godeye --help' for more information");
		exit(EXIT_SUCCESS);
	}
	return options_bool;
}
