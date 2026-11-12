#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpuid.h>

#include "file_operations.h"
#include "list.h"
#include "input.h"
#include "output.h"

void list_item_add(char **l, size_t cap)
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

void list_load(char **l, size_t cap, FILE *f)
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

void list_save(char **l, size_t cap)
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

void list_view(char **l, size_t cap)
{
	printf("\n\n====== START LIST ======\n");
	out_numbered_list(l, cap);
	printf("====== ENDOF LIST ======\n\n");
}

size_t list_size(char **l, size_t cap)
{
	size_t i = 0;
	while (i < cap && l[i][0] != '\0') { i++; }

	return i;
}

void list_item_delete(char **list, size_t cap)
{
	list_view(list, cap);
	size_t size = list_size(list, cap); // get item count

	char message[32];
	snprintf(message, sizeof(message), "Delete item [1-%lu]: ", size); // Load string into message (pointer to block of 32 bytes)

	int deleteIdx = get_digit(message);
	while (0 >= deleteIdx || cap <= deleteIdx) { // must be between or equal to 1 and/or cap
		deleteIdx = get_digit(message);
	}
	
	deleteIdx--;
	
	// make sure to save the pointer before zero-ing it's value
	char *freedSlot = list[deleteIdx];
	freedSlot[0] = '\0'; // Set value at address to null terminator. Ending the string early (Value is not removed, just ended early)
	// The data isn't deleted, and the memory block still exists. We just tell the system the string has length 0.
	// Then we move the position of the pointer to that memory block to the end of our "List".
	// That way, it becomes our definition of "empty" and the block can be overwritten when necessary
	for (size_t i = deleteIdx; i < cap - 1; i++) {
		list[i] = list[i + 1]; // Shift all items to the left. Setting current to next.
	}

	list[cap - 1] = freedSlot; // Make sure we append the "empty" pointer.
}
