#include <stdio.h>
#define FILE_READ_ERROR -1
#define FILE_TOO_LARGE_ERROR -2

void list_item_add(char **list, size_t cap);
void list_load(char **list, size_t cap, FILE *f);
void list_save(char **list, size_t cap);
void list_view(char **list, size_t cap);
void list_item_delete(char **list, size_t cap);

size_t list_size(char **list, size_t cap);
