#include <stdio.h>
#include <string.h>

#include "input.h"

int get_digit(char *msg)
{
	int a;
	char b[3];

	get_line(b, sizeof(b), msg);
	b[strcspn(b, "\n")] = '\0';

	sscanf(b, "%d", &a);

	return a;
}

void get_line(char *b, size_t b_size, char *msg)
{
	printf("%s", msg);
	fgets(b, b_size, stdin);
}

