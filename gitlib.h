#include "clist.h"
#include "hashlib.h"
#ifndef GITLIB
#define GITLIB

List* listdir(char* root_dir);
int file_exists(char* file);
void cp(char *to, char* from);
void blobFile(char* file);

#endif
