#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_operations.h"
#include "list.h"
#include "input.h"

void list_item_add(char l[][128], size_t cap)
{
	size_t pos = 0;
	while (pos < cap && l[pos][0] != '\0') {
		pos++;
	}

	if (pos == cap) {
		printf("List is full\n");
		return;
	}

	char line[128] = {0};
	fgets(line, 128, stdin);
	line[strcspn(line, "\n")] = '\0'; // strcspn = Get amount of characters before second arg. Simply put: Get index of second arg.
	strcpy(l[pos], line);
}

void list_load(char l[][128], size_t cap, FILE *f)
{
	size_t bytes = file_get_size(f);

	char buf[bytes+1];
	buf[bytes] = '\0';

	size_t bytesRead = fread(buf, sizeof(char), bytes, f);

	char line[128] = {0};
	size_t listCounter = 0;
	size_t lineCounter = 0;

	for (size_t i = 0; i < bytesRead; i++) {
		if (listCounter == cap || lineCounter == sizeof(line)-1) {
			printf("File is not valid. List or lines are too large\n");
			return;
		}

		line[lineCounter++] = buf[i];

		if ('\n' == buf[i]) {
			line[lineCounter-1] = '\0'; // replace previously set (\n) character with null terminator
			strcpy(l[listCounter++], line); // copy to list
			memset(line, 0, sizeof(line)); // Reset line
			lineCounter = 0;
		}
	}

	if (lineCounter > 0 && listCounter < cap) {
		strcpy(l[listCounter++], line); // last line had no trailing newline
	}
}

void list_save(char l[][128], size_t cap)
{
	FILE *f = fopen("data.txt", "w");

	if (!f) {
		exit(FILE_READ_ERROR);
	}

	for (size_t i = 0; i < cap; i++) {
		if (l[i][0] == '\0') break;

		fprintf(f, "%s\n", l[i]);
	}

	fclose(f);
}
