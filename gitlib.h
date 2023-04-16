#include "clist.h"
#include "hashlib.h"
#ifndef GITLIB
#define GITLIB

List* listdir(char* root_dir);
int file_exists(char* file);
void cp(char *to, char* from);
void blobFile(char* file);
int getChmod(const char* path);
void setMode(int mode, char* path);
char* concat_paths(char* p1, char* p2);
int isFile(const char* path);

#endif
