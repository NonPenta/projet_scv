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
void createFile(char* file);
void myGitAdd(char* file_or_folder);
void myGitCommit(char* branch_name, char* message);
void myGitCheckoutBranch(char* branch);

#endif
