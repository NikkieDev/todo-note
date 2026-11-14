#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpuid.h>

#include "file_operations.h"
#include "list.h"
#include "input.h"
#include "output.h"
#include "memory.h"

// char * or char whatever[16] are both just a pointer to a starting address, with the possibility of an offset.
// whatever[16] says "I point to a block of 16 bytes". char * says "I point to a block of memory."
// That's why char * requires malloc. You need to give it a memory block to point at.
// Array's don't exist. You point to a starting address, and you can use an offset to go further in the "array"
// Thats why 2[arr] is as valid as arr[2]. You're just grabbing starting address + 2, or 2 + starting address.
// whatever[16] doesn't really have a pointer, you are actually working directly with the value.
// But whatever[0] just points at the value stored at the starting address.
// I love CPU's

static void list_free(List *list)
{
	free(list->storage);
	free(list->items);
}

static void item_add(List *list, char *item)
{
	strcpy(list->storage[list->count], item);
	list->items[list->count] = list->storage[list->count];
	list->count++;
}

void list_init(List *list, int items)
{
	int itemSize = 128;
	int capacitySize = itemSize * items;
	list->storage = allocate(capacitySize);
	list->items = allocate(items * sizeof(char *)); // we need 8 bytes (size of a pointer to a character) for every item

	// -> follows the pointer to the address of the value. When using pointer.
	// . changes the value directly. Only possible when using the real struct
	for (size_t i =0; i< items; i++) {
		list->items[i] = list->storage[i];
	}

	list->capacity = items;
	list->count = 0;
}

void list_load(List *list, FILE *f)
{
	size_t bytes = file_get_size(f);

	char buf[bytes+1];
	size_t bRead = fread(buf, sizeof(char), bytes, f);

	char line[128] = {0};
	int lineCounter = 0;

	for (size_t i = 0; i < bRead; i++) {
		if (list->count == list->capacity || lineCounter == sizeof(line)-1) {
			printf("File is not valid. List or lines are too large\n");
			return;
		}

		if ('\n' == buf[i]) { // Because char is never added to line and line is nullified, no need to set a null terminator.
			item_add(list, line);
			memset(line, 0, sizeof(line));
			lineCounter = 0;

			continue;
		}

		line[lineCounter++] = buf[i];
	}

	if (lineCounter > 0 && list->count < list->capacity) {
		item_add(list, line);
	}
}

void list_item_add(List *list)
{
	if (list->count == list->capacity) {
		printf("List is full\n");
		return;
	}

	char line[128] = {0};
	fgets(line, 128, stdin);
	line[strcspn(line, "\n")] = '\0'; // strcspn = Get amount of characters before second arg. Simply put: Get index of second arg.
	item_add(list, line);
}

void list_item_delete(List *list)
{
	list_view(*list);

	char message[32];
	snprintf(message, sizeof(message), "Delete item [1-%lu]: ", list->count-1); // Load string into message (pointer to block of 32 bytes)
	
	int deleteIdx = get_digit(message);
	while (0 >= deleteIdx || list->count < deleteIdx) {
		deleteIdx = get_digit(message);
	}

	deleteIdx--;
	// make sure to save the pointer before zero-ing it's value
	char *freedSlot = list->items[deleteIdx];
	freedSlot[0] = '\0'; // Set value at address to null terminator. Ending the string early (Value is not removed, just ended early)
	// The data isn't deleted, and the memory block still exists. We just tell the system the string has length 0.
	// Then we move the position of the pointer to that memory block to the end of our "List".
	// That way, it becomes our definition of "empty" and the block can be overwritten when necessary
	for (size_t i = deleteIdx; i < list->count - 1; i++) {
		list->items[i] = list->items[i+1]; // Shift all items to the left. Setting current to next.
	}

	list->items[list->count-1] = freedSlot;
	// Make sure we append the "empty" pointer. By setting it to the position of the last existing element
	// because the last element was never overwritten by a next item being moved back.
	// So we write to it's place

}

void list_save(List list)
{
	FILE *f = fopen("data.txt", "w");

	if (!f) {
		exit(FILE_READ_ERROR);
	}

	for (size_t i = 0; i < list.capacity; i++) {
		if (list.items[i][0] == '\0') break;
		fprintf(f, "%s\n", list.items[i]);
	}

	fclose(f);
}

void list_view(List list)
{
	printf("\n\n====== START LIST ======\n");
	out_numbered_list(list.items, list.capacity);
	printf("====== ENDOF LIST ======\n\n");
}

