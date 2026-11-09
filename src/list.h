#include <stdio.h>
#define FILE_READ_ERROR -1
#define FILE_TOO_LARGE_ERROR -2

void list_item_add(char l[][128], size_t cap);
void list_load(char l[][128], size_t cap, FILE *f);
void list_save(char l[][128], size_t cap);
