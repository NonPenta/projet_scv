#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "commit.h"
#include "hashlib.h"

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

void commitSet(Commit* c, char* key, char* value){
	
	if (c->n >= c->size) {
		printf("Erreur: la taille max du commit est atteinte");
		return;
	}
	int p = (int) (djb2(key) % c->size);
	while (c->T[p] != NULL) {
		p = (p+1) % c->size;
	}
	
	c->T[p] = createKeyVal(key, value);
	c->n++;
}

Commit* createCommit(char* hash){
	Commit* c = initCommit();
	commitSet(c, "tree", hash);
	return c;
}

char* commitGet(Commit* c, char* key){
	int p = (int) (djb2(key) % c->size);
	int recherche = 0;
	while (c->T[p] != NULL && recherche < c->size) {
		if (strcmp(c->T[p]->key, key) == 0) {
			return c->T[p]->value;
		}
		p = (p+1)%c->size;
		recherche++;
	}
	
	return NULL;
}


