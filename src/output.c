#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "output.h"

void out_numbered_list(char **list, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		if (strlen(list[i]) == 0) break;
		printf("%lu) %s\n", i+1, list[i]);
	}
}
