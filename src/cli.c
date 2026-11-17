#include "cli.h"
#include "memory.h"
#include <stdio.h>
#include <string.h>

int cli_start(int argc, char **argv)
{
	char *programName = argv[0];
	char *body = allocate(1024);

	for (int i = 1; i < argc; i++) {
		if (1000 <= strlen(body)) break;

		char part[128] = {0};
		snprintf(part, 128, "%s ", argv[i]);

		strcat(body, part);
	}
	body[strlen(body)] = '\n';

	FILE *f = fopen("./data.txt", "a");
	if (!f) {
		return 1;
	}

	fwrite(body, strlen(body), 1, f);
	fclose(f);

	return 0;
}
