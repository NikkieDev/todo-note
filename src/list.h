#include <stdio.h>
#define FILE_READ_ERROR -1
#define FILE_TOO_LARGE_ERROR -2

typedef struct {
	char (*storage)[128]; // Define a pointer to N blocks each 128 bytes in size.
	char **items;
	size_t count;
	size_t capacity;
} List;
// typedef makes an alias. All structs have to be called as "struct Object", making an alias to "Object"
// allows the compiler to see "Object" as the struct datatype since it is an alias to "struct Object"

static void list_free(List *list);

void list_init(List *list, int items);
void list_load(List *list, FILE *f);
void list_item_add(List *list);
void list_item_delete(List *list);
void list_save(List list);
void list_view(List list);
