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
			printf("Usage: ./ordereye-parser [options] [file] [pattern] [count] [position] [substring] [count]\n\n");

			printf("Options:\n");
			printf("  -l       Normal mode (like grep). Prints the entire line if it contains the specified pattern.\n");
			printf("  -p       Extraction mode. Extracts everything after the position until the pattern is found or after a certain number of occurrences.\n");
			printf("  -s       Substring extraction mode. Extracts everything after the specified substring until it finds the specified pattern a given number of times.\n");
			printf("  -v       Invert the result for `-l` mode, showing lines that do NOT contain the specified pattern.\n");
			printf("  -e       Enable enumeration. For `-l`, `-s`, and `-p` modes, this will display the line number alongside the results.\n\n");

			printf("Parameters:\n");
			printf("  file     The file you want to process.\n");
			printf("  pattern  The pattern or substring you want to search for.\n");
			printf("  count    The number of occurrences of the pattern or substring to consider for `-p` and `-s` modes.\n");
			printf("  position The starting position in the string for `-p` mode (applicable only for `-p` mode).\n");
			printf("  substring The substring used as the delimiter for extraction (applicable to `-p` and `-s` modes).\n\n");

			printf("Examples:\n");
			printf("1. Using `-l` mode with enumeration:\n");
			printf("   ./ordereye-parser -le test.txt 'substring'\n\n");

			printf("2. Using `-p` mode to extract after position 1 until the second occurrence of a space:\n");
			printf("   ./ordereye-parser -pe test.txt 1 ' ' 2\n\n");

			printf("3. Using `-s` mode to extract everything after 'substring' until the pattern '.' appears 2 times:\n");
			printf("   ./ordereye-parser -se test2.txt 'sustring' 1 '.' 2\n\n");

			printf("4. Using `-v` with `-l` to exclude lines containing 'substring':\n");
			printf("   ./ordereye-parser -l -v test2.txt 'substring'\n");
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
