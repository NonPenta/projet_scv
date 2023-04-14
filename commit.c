#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "commit.h"

kvp* createKeyVal(char* key, char* val) {
	kvp* kv = (kvp*) malloc(sizeof(kvp));
	kv->key = strdup(key);
	kv->value = strdup(val);
	
	return kv;
	
}

void freeKeyVal(kvp* kv) {
	free(kv->key);
	free(kv->value);
	free(kv);
}

char* kvts(kvp* kv) {
	char* skv = (char*) malloc((strlen(kv->key) + strlen(kv->value) + 1 + 1) * sizeof(char));
	sprintf(skv, "%s : %s", kv->key, kv->value);
	return skv;
}

kvp* stkv(char* str) {
	char key[100], val[100];
	sscanf(str, "%s : %s", key, val);
	return createKeyVal(key, val);
}

Commit* initCommit() {
	Commit* c = malloc(sizeof(Commit));
	c->T = malloc(SIZE_COMMIT * sizeof(kvp *));
	c->size = SIZE_COMMIT;
	
	for (int i = 0; i < SIZE_COMMIT; i++) {
		c->T[i] = NULL;
	}
	
	c->n = 0;
	
	return c;
}


