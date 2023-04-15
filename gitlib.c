#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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

void blobFile(char* file) {
	if (file_exists(file)) {
		char* hash = sha256file(file);

		char* dir = strdup(hash); dir[2] = '\0';
		char* path = hashToPath(hash);

		if (!file_exists(dir)) {
			char command[9];
			sprintf(command, "mkdir %c%c", path[0], path[1]);
			system(command);
		}

		free(dir);
		cp(path, file);
	}
}

int getChmod(const char* path) {
	struct stat ret;
	if (stat(path, &ret) == -1) {
		return -1;
	}
	return 	(ret.st_mode &S_IRUSR)|(ret.st_mode &S_IWUSR)|(ret.st_mode &S_IXUSR)|/* owner*/
				(ret.st_mode &S_IRGRP)|(ret.st_mode &S_IWGRP)|(ret.st_mode &S_IXGRP)|/* group*/
				(ret.st_mode &S_IROTH)|(ret.st_mode &S_IWOTH)|(ret.st_mode &S_IXOTH);/* other*/
}

char* concat_paths(char* p1, char* p2) {
	char* res = malloc(strlen(p1) + strlen(p2) + 2);
	if (res == NULL) { printf("Unable to allocate memory (for god knows what reason)"); return NULL; }
	sprintf(res, "%s/%s", p1, p2);
	return res;
}

int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
