#pragma once 

#include <stdio.h>
#include "cli_utils.h"

#define MAXLINE         2048 

typedef struct {
	char find;      // The delimiter character to find
	int findtimes;  // The number of times the delimiter should appear
}find_info;

typedef struct {
	char *substr;     // Substring to search for
	int substr_times;  // Number of times the substring should appear
}substr_info;

typedef struct {
	int line;  // Line number in the filtered matrix
	int size;  // Matrix size
}matrix_info;

void line_filter(char *line);
char *poscollect (char *str, int position, find_info find);
char *substrcollect (char *str, substr_info substr, find_info find);
void printlines (char (*matrix)[MAXLINE], int capacity, int *ocurrences, bool e_check);
int count_lines (FILE *file);
char (*strparse (shell_info shell_args, int capacity, char *substr, bool_flag options_bool))[MAXLINE];
char (*posparse (shell_info shell_args, int position, int capacity, find_info find))[MAXLINE];
char (*substrparse (shell_info shell_args, int capacity, substr_info substr, find_info find))[MAXLINE];
char (*matrix_filter (char (*filelines)[MAXLINE], char (*filteredlines)[MAXLINE], int capacity))[MAXLINE];
void line_ocurrences (char (*filelines)[MAXLINE], char (*filteredlines)[MAXLINE], int *ocurrences);
char (*enumerate (char (*lines)[MAXLINE], int capacity, int *ocurrences))[MAXLINE];
