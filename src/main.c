#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_operations.h"
#include "list.h"
#include "input.h"

void menu()
{
	int major = 0x0;
	int minor = 0x1;
	// 3 * 16 = 48 bytes
	const char menu[3][0x10] = {"Add item", "View list", "Exit"};

	printf("Welcome to TodoList V%d.%d\n", major, minor);
	for (size_t i = 0; i < sizeof(menu) / sizeof(menu[0]); i++) {
		printf("%lu) %s\n", i+1, menu[i]);
	}
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

	char l[20][128] = {0};
	size_t capacity = sizeof(l)/sizeof(l[0]);

	list_load(l, capacity, f);
	fclose(f);

	int choice = 0;
	while (3 != choice) {
		menu();

		choice = get_digit("Select an option: ");
		printf("Selected: %d\n", choice);

		if (1 == choice) {
			list_item_add(l, capacity);
		} else if (2 == choice) {
			printf("\n\n====== START LIST ======\n");
			for (size_t i = 0; i < capacity; i++) {
				if (strlen(l[i]) == 0) break;

				printf("%lu) %s\n", i+1, l[i]);
			}
			printf("====== ENDOF LIST ======\n\n");
		}
	}

	list_save(l, capacity);

	return 0;
}
