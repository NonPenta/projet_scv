#ifndef REFLIB
#define REFLIB

void initRefs();
void createUpdateRef(char* ref_name, char* hash);
void deleteRef(char* ref_name);
char* getRef(char* ref_name);

#endif