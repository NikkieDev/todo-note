#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_operations.h"
#include "list.h"
#include "input.h"
#include "output.h"

void menu()
{
	int major = 0x0;
	int minor = 0x1;
	char *menu[4] = {"Add item", "Delete item", "View list", "Exit"};
	char **p = menu;

	printf("Welcome to TodoList V%d.%d\n", major, minor);
	out_numbered_list(p, 4);
}

int main(void)
{
	FILE *f = fopen("data.txt", "r");

	if (!f) {
		exit(FILE_READ_ERROR);
	}

	size_t bytes = file_get_size(f);
	if (10000000 <= bytes) { // Don't allow more than 10mb
		printf("File too large, please reset\n");
		exit(FILE_TOO_LARGE_ERROR);
	}

	char storage[20][128] = {0};
	char *l[20];
	for (size_t i = 0; i < 20; i++) {
		l[i] = storage[i];
	}
	size_t capacity = sizeof(l)/sizeof(l[0]);

	list_load(l, capacity, f);
	fclose(f);

	int choice = 0;
	while (4 != choice) {
		menu();

		choice = get_digit("Select an option: ");
		printf("Selected: %d\n", choice);

		if (1 == choice) {
			list_item_add(l, capacity);
		} else if (2 == choice) {
			list_item_delete(l, capacity);
		} else if (3 == choice) {
			list_view(l, capacity);
		}
	}

	list_save(l, capacity);

	return 0;
}
