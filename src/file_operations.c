#include "file_operations.h"
#include <stdio.h>
#include <string.h>

// Get linecount of currently open filestream
int file_get_linecount(FILE *f)
{
	int ch, lines = 0;

	while (!feof(f)) {
		if ((ch = fgetc(f)) == '\n') {
			++lines;
		}
	}

	rewind(f);
	return lines;
}

size_t file_get_size(FILE *f)
{
	fseek(f, 0, SEEK_END);
	size_t bytes = ftell(f);
	rewind(f);

	return bytes;
}
