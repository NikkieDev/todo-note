#include "memory.h"
#include <stdlib.h>
#include <string.h>

void *allocate(int bytes)
{
	void *ptr = malloc(bytes);
	if (NULL != ptr) {
		memset(ptr, 0, bytes);
	}

	return ptr;
}
