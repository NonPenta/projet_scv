#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include "hashlib.h"
#include "gitlib.h"
#include "clist.h"

List* listdir(char* root_dir) {
	List* l = initList();
	DIR* dp = opendir(root_dir);
	struct dirent* ep;
	if (dp == NULL) { return l; }
	while ((ep = readdir(dp))) {
		Cell* c = buildCell(ep->d_name);
		insertFirst(l, c);
	}

	closedir(dp);
	return l;
}

int file_exists(char* file) {
	FILE *pwd;
	pwd = popen("pwd", "r");
	char dir[128];
	fscanf(pwd, "%s\n", dir);
	pclose(pwd);

	List* l = listdir(dir);

	Cell* c = *l;

	while (c) {
		if (strcmp(file, c->data) == 0) return 1;
		c = c->next;
	}

	freeList(l);
	return 0;
}

void cp(char* to, char* from) {
	if (file_exists(from)) {
		FILE* ffrom = fopen(from, "r");
		FILE* fto = fopen(to, "w");

		char line[128];
		while (fgets(line, 128, ffrom)) {
			fputs(line, fto);
		}
	}
}

char* hashToPath(char* file) {
	if (file_exists(file)) {
		char* hash = sha256file(file);
		char* path = malloc(65 * sizeof(char));
		sprintf(path, "%c%c/%s", hash[0], hash[1], hash+2);
		return path;
	}
	return "file doesnt exist";
}

void blobFile(char* file) {
	if (file_exists(file)) {
		char* path = hashToPath(file);
		char command[9];
		sprintf(command, "mkdir %c%c", path[0], path[1]);
		system(command);
		cp(path, file);
	}
}
