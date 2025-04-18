#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parse_utils.h"
#include "cli_utils.h"

void line_filter(char *line)
{
	for (int i = 0; line[i] != '\0'; i++) 
	{

		if (line[i] == '\n') 
		{
			line[i] = ' '; // Replace newline characters with spaces
		}
	}
}

// Filter the file based on an initial position and a delimiter character.
// The delimiter character are user-specified, which limits the scope of the filtering.

char *poscollect (char *str, int position, find_info find)
{
	int find_count = 0;
	char *collect = (char *)malloc(MAXLINE * sizeof(char));

	for (int i = position, collect_i = 0; str[i] != '\0'; i++, collect_i++) 
	{
		if(str[i] == '\n') 
		{
			str[i] = ' ';
		}

		if (find_count == find.findtimes) 
		{
			collect[i] = '\0';
			break;
		}

		if (str[i] == find.find) 
		{
			++find_count;
		}

		if (i == strlen(str) - 1 && find_count != find.findtimes) 
		{
			strcpy(collect, "");
			break;
		}
		collect[collect_i] = str[i];
	}

	return collect;

}

// Filter the file based on a substring and a delimiter character.
// Both the substring and delimiter character are user-specified, which limits the scope of the filtering.

char *substrcollect (char *str, substr_info substr, find_info find)
{
	int substr_pos = 0, substr_count = 0;
	char *collect = (char *)malloc(MAXLINE * sizeof(char));

	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == substr.substr[substr_pos]) 
		{
			++substr_pos;
		}

		if (substr_pos == strlen(substr.substr)) 
		{
			substr_pos = 0;
			++substr_count;
		}

		if (substr_count == substr.substr_times) 
		{
			collect = poscollect(str, i+1, find); // Collect from the next position
			break;
		}

		if(i == strlen(str) - 1 && substr_count != substr.substr_times)
		{
			strcpy(collect, "");
			break;
		}
	}
	return collect;
}

// Print all the filtered lines from the file according to the selected mode.
// Without using the -e mode, only the lines that match the filter criteria will be printed.
// With the -e mode, print occurrences of identical strings.

void printlines (char (*filelines)[MAXLINE], int capacity, int *ocurrences, bool e_check)
{
	if (filelines) 
	{
		for (int i = 0;i < capacity; i++) 
		{
			if (filelines[i][0] != '\0' && e_check) 
			{
				printf("\033[31m%d:\033[m%s\n",ocurrences[i], filelines[i]); // Print the occurrences with -e option
			} else if (filelines[i][0] != '\0' && !e_check) 
			{
				printf("%s\n", filelines[i]);
			}
		}
	}
}

// Count the total number of lines in the file, so they can be iterated over later.

int count_lines (FILE *file)
{
	int lines = 0;
	char *line = (char *)malloc(MAXLINE * sizeof(char));
	while(fgets(line, MAXLINE, file)){
		++lines;
	}
	free(line);
	rewind(file);
	return lines;
}

// Search for substring occurrences in a file, line by line.
// After that, if the filter result is not NULL, add the filtered string to the result matrix.

char (*strparse (shell_info shell_args, int capacity, char *substr, bool_flag options_bool))[MAXLINE]
{
	matrix_info matrix = {0, capacity};
	char (*parselines)[MAXLINE] = malloc(matrix.size * sizeof(*parselines));
	alloccheck(parselines);

	char *filterline = (char *)malloc(MAXLINE * sizeof(char));
	alloccheck(filterline);
	while (fgets(filterline, MAXLINE, shell_args.file)) {
		if (strstr(filterline, substr)) // IMPLEMENTAR A LÓGICA DO -V AQUI
		{
			if (options_bool.v_check)
			{
				continue;
			}

			line_filter(filterline);
			strcpy(parselines[matrix.line], filterline);
			++matrix.line;
		}else
		{
			if (options_bool.v_check)
			{
				line_filter(filterline);
				strcpy(parselines[matrix.line], filterline);
				++matrix.line;
			}
		}
	}
	strcpy(parselines[matrix.line], "\0");
	return parselines;
}

// Filter the file based on an initial position and a delimiter character.
// The delimiter character are user-specified, which limits the scope of the filtering.

char (*posparse (shell_info shell_args, int position, int capacity, find_info find))[MAXLINE]
{

	matrix_info matrix = {0, capacity};
	char (*parselines)[MAXLINE] = malloc(matrix.size * sizeof(*parselines));
	alloccheck(parselines);

	char *filterline = (char *)malloc(MAXLINE * sizeof(char));
	alloccheck(filterline);
	while (fgets(filterline, MAXLINE, shell_args.file)) {
		if ((filterline = poscollect(filterline, position, find))) 
		{
			line_filter(filterline);
			strcpy(parselines[matrix.line], filterline);
			++matrix.line;
		}
	}
	strcpy(parselines[matrix.line], "\0");
	return parselines;
}

// Filter the file based on a substring and a delimiter character.
// Both the substring and delimiter character are user-specified, which limits the scope of the filtering.

char (*substrparse (shell_info shell_args, int capacity, substr_info substr, find_info find))[MAXLINE]
{
	matrix_info matrix = {0, capacity};
	char (*parselines)[MAXLINE] = malloc(matrix.size * sizeof(*parselines));
	alloccheck(parselines);

	char *filterline = (char *)malloc(MAXLINE * sizeof(char));
	alloccheck(filterline);
	while (fgets(filterline, MAXLINE, shell_args.file)) {
		if ((filterline = substrcollect(filterline, substr, find)) && filterline[0] != '\0') 
		{
			line_filter(filterline);
			strcpy(parselines[matrix.line], filterline);
			++matrix.line;
		}
	}
	strcpy(parselines[matrix.line], "\0");
	return parselines;
}

// Search for occurrences of identical strings in the filter result matrix and create an array of occurrence counts.

char (*matrix_filter (char (*filelines)[MAXLINE], char (*filteredlines)[MAXLINE], int capacity))[MAXLINE]
{

	int line = 0, filtered_size = 0;

	for (int i = 0; strcmp(filelines[i], "\0") != 0; i++) {
		bool not_found = true;

		for (int i2 = 0; i2 < filtered_size; i2++) {
			if (strcmp(filelines[i], filteredlines[i2]) == 0)
			{
				not_found = false;
				break;
			}
		}
		if (not_found) 
		{
			strcpy(filteredlines[line], filelines[i]);
			++line;
			++filtered_size;
		}
	}
	strcpy(filteredlines[line], "\0");
	return filteredlines;

}

// Search for equal string ocurrences in filter result matrix and create an array of ocurrence's number.

void line_ocurrences (char (*filelines)[MAXLINE], char (*filteredlines)[MAXLINE], int *ocurrences)
{

	for (int i = 0; strcmp(filteredlines[i], "\0") != 0; i++) 
	{
		for (int i2 = 0; strcmp(filelines[i2], "\0") != 0; i2++) 
		{
			if (strcmp(filteredlines[i], filelines[i2]) == 0) 
			{
				++ocurrences[i];
			}
		}

	}
}

// enumerate all the identical ocurrences of the string in result matrix.

char (*enumerate (char (*filelines)[MAXLINE], int capacity, int *ocurrences))[MAXLINE]
{
	int line = 0, filtered_size = 0;
	char (*filteredlines)[MAXLINE] = malloc(capacity * sizeof(*filteredlines));
	alloccheck(filteredlines);
	filteredlines = matrix_filter(filelines, filteredlines, capacity);

	line_ocurrences(filelines, filteredlines, ocurrences);

	return filteredlines;
}
