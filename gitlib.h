#include "clist.h"
#ifndef GITLIB
#define GITLIB

List* listdir(char* root_dir);
int file_exists(char* file);
void cp(char *to, char* from);
char* hashToPath(char* hash);
void blobFile(char* file);

#endif
