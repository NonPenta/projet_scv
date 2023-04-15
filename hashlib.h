#ifndef HASHLIB
#define HASHLIB

int hashFile(char* source, char* dest);
char* sha256file(char* file);
char* hashToPath(char* hash);
char* hashToFile(char* hash);
char* djb2(char* str);

#endif
