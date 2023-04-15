#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "hashlib.h"
#include "gitlib.h"

int hashFile(char *source, char *dest) {
  char command[19 + strlen(source) + strlen(dest)];

  sprintf(command, "cat %s | sha256sum > %s", source, dest);
  system(command);
  return 1;
}

char* sha256file(char* file) {
	char fname[22] = "/tmp/sha256fileXXXXXX";
	mkstemp(fname);
	char command[19 + strlen(file) + sizeof(fname)];

	sprintf(command, "cat %s | sha256sum > %s", file, fname);
	system(command);

	FILE* f = fopen(fname, "r");
	char* hash = malloc(64 * sizeof(char));
	fgets(hash, 64, f);

	sprintf(command, "rm %s", fname);
	system(command);

	fclose(f);
	return hash;
}

char* hashToPath(char* hash) {
	char* path = malloc(65 * sizeof(char));
	sprintf(path, "%c%c/%s", hash[0], hash[1], hash+2);
	return path;
}

char* hashToFile(char* hash) {
	char* dir = strdup(hash); dir[2] = '\0';
	if (!file_exists(dir)) {
			char command[9];
			sprintf(command, "mkdir %s", dir);
			system(command);
	}
	free(dir);
	return hashToPath(hash);
}

unsigned long djb2(char* str) {
  unsigned long hash = 5381;
  int c;
  while (c = *str++) {
    hash = ((hash << 5) + hash) + c;
  }
  
  return hash;
}
