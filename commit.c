#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "commit.h"
#include "hashlib.h"
#include "gitlib.h"

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
	char* skv = (char*) malloc((strlen(kv->key) + strlen(kv->value) + 1 + 1 + 1) * sizeof(char));
	sprintf(skv, "%s : %s\n", kv->key, kv->value);
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
	
	printf("Commit initialisÈ.\n");
	
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
	printf("Commit crÈÈ.\n");
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

char* cts(Commit* c) {
	char* str = malloc(sizeof(char) * 100 * c->n);
	for (int i = 0; i < c-> size; i++) {
		if (c->T[i] != NULL) {
			strcat(str, kvts(c->T[i]));
		}
	}
	return str;
}

Commit* stc(char* str) {;
	Commit* c = initCommit();
	char* buffer = (char*) malloc(200 * sizeof(char));
	sscanf(str, "%[^\n]", buffer);
	while(strchr(str, '\n')) {
		char key[100];
		char value[100];
		sscanf(str, "%s : %s\n", key, value);
		commitSet(c, key, value);
		str = strcpy(str, str + strlen(buffer) + 1);
		sscanf(str, "%[^\n]", buffer);
	}
	free(buffer);
	return c;
	
}

void ctf(Commit* c, char* file){
	FILE* f = fopen(file, "w");
	if (f != NULL) {
		fprintf(f, "%s", cts(c));
		fclose(f);
	} else {
		printf("Erreur lors de la cr√©ation du fichier %s.\n", file);
	}
}

Commit* ftc(char* file){
	FILE* f = fopen(file, "r");
	if (f == NULL) {
		printf("Erreur lors de l'ouverture du fichier %s.\n", file);
		return NULL;
	}
	
	Commit* c = initCommit();
	
	char* buffer = (char*) malloc(200 * sizeof(char));
	
	while(fgets(buffer, 200, f) != NULL) {
		char key[100];
		char value[100];
		
		sscanf(buffer, "%s : %s\n", key, value);
		commitSet(c, key, value);
		
	}
	
	free(buffer);
	fclose(f);
	
	return c;
}

char* blobCommit(Commit* c) {
	char fname[14] = "/tmp/coXXXXXX";
	mkstemp(fname);
	ctf(c, fname);
	char* hash = sha256file(fname);
	char* cbf = hashToFile(hash);
	cbf = realloc(cbf, 67);
	strcat(cbf, ".t");
	cp(cbf, fname);
	char rm[17] = "rm ";
	strcat(rm, fname);
	system(rm);
	return hash;
}


