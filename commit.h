#ifndef COMMIT
#define COMMIT

#define SIZE_COMMIT 100

typedef struct key_value_pair{
	char* key;
	char* value;
} kvp;

typedef struct hash_table {
	kvp** T;
	int n;
	int size;
} HashTable;

typedef HashTable Commit;

kvp* createKeyVal(char* key, char* val);
void freeKeyVal(kvp* kv);
char* kvts(kvp* kv);
kvp* stkv(char* str);
Commit* initCommit();

#endif
