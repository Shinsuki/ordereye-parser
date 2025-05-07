#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cli_utils.h"
#include "parse_utils.h"

int main (int argc, char **argv)
{
	if (argc < 4)
	{
		fprintf(stderr, "Too few arguments provided");
		exit(EXIT_FAILURE);
	}

	shell_info shell_args;
	find_info find;
	substr_info substr;
	bool_flag options_bool = {false, false, false, false}; // Initialize options flags
	shell_args.file = NULL;
	shell_args.arguments = malloc(100 * sizeof(char)); // Allocate memory for arguments
	shell_args.arguments_count = (int *)malloc(sizeof(int)); // Allocate memory for argument count
	alloccheck(shell_args.arguments);
	shell_args.options = (char *)malloc(MAXLINE * sizeof(char));  // Allocate memory for options
	alloccheck(shell_args.options);

	help(argv);
	processargs(argc, argv, &shell_args);
	int capacity = count_lines(shell_args.file); // Get the number of lines in the file
	
	options_bool = checkoptions(shell_args.options, options_bool);
	int *ocurrences = (int *)calloc(capacity, sizeof(int));  // Allocate memory to count occurrences of each line

	// Handle the different options based on the passed arguments
	if (options_bool.l_check && *shell_args.arguments_count == 1) 
	{
		substr.substr = argv[3];
		char (*parselines)[MAXLINE] = strparse(shell_args, capacity, substr.substr, options_bool);
		if (options_bool.e_check) 
		{
			parselines = enumerate(parselines, capacity, ocurrences);
		}
		printlines(parselines, capacity, ocurrences, options_bool.e_check);

	} else if (options_bool.s_check && *shell_args.arguments_count == 4) 
	{
		substr.substr = argv[3];
		substr.substr_times = atoi(argv[4]);
		find.find = argv[5][0];
		find.findtimes = atoi(argv[6]);

		// Check if the times for the substring and delimiter are valid numbers
		if (!check_arg(shell_args.arguments[1]) || !check_arg(shell_args.arguments[3])) 
		{
			fprintf(stderr, "You need to put digits in the finding substring times and the delimitator char find\nTry 'ordereye-parser --help'for more information.");
			exit(EXIT_FAILURE);
		}

		char (*parselines)[MAXLINE] = substrparse(shell_args, capacity, substr, find);

		if (options_bool.e_check) 
		{
			parselines = enumerate(parselines, capacity, ocurrences);
		}
		printlines(parselines, capacity, ocurrences, options_bool.e_check);
	} else if (options_bool.p_check && *shell_args.arguments_count == 3) 
	{
		int position = atoi(argv[3]);        // Position to start collecting from
		find.find = argv[4][0];              // Parse delimiter character
		find.findtimes = atoi(argv[5]);      // Parse number of delimiter occurrences

		if (!check_arg(shell_args.arguments[0]) || !check_arg(shell_args.arguments[2])) 
		{
			fprintf(stderr, "You need to put digits in the finding substring times and the delimitator char find\nTry 'ordereye-parser --help'for more information.");
			exit(EXIT_FAILURE);
		}

		char (*parselines)[MAXLINE] = posparse(shell_args, position, capacity, find);

		if (options_bool.e_check) 
		{
			parselines = enumerate(parselines, capacity, ocurrences);
		}
		printlines(parselines, capacity, ocurrences, options_bool.e_check);

	} else 
	{
		fprintf(stderr, "Invalid Option"); // Invalid options
		exit(EXIT_FAILURE);
	}
}
