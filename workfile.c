#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "workfile.h"
#include "hashlib.h"
#include "gitlib.h"

WorkFile* createWorkFile(char* name) {
	WorkFile* wf = (WorkFile*) malloc(sizeof(WorkFile));
	wf->name = strdup(name);
	wf->hash = NULL;
	wf->mode = 0;
	
	return wf;
}

char* wfts(WorkFile* wf) {
	char* hash;
	if (wf->hash != NULL) {
		hash = strdup(wf->hash);
	} 
	char* wfts = (char*) malloc((6 + strlen(wf->name) + strlen(hash)) * sizeof(char));
	sprintf(wfts, "%s\t%s\t%d\n", wf->name, wf->hash, wf->mode);
	return wfts;
	
}

WorkFile* stwf(char* ch) {
	int mode;
	char hash[1000];
	char nom[1000];
	WorkFile* wf = createWorkFile("temp");
	sscanf(ch, "%s\t%s\t%d\n", nom, hash, &mode);
	wf->mode = mode;
	wf->name = strdup(nom);
	wf->hash = strdup(hash);
	return wf;
}

WorkTree* initWorkTree() {
	WorkTree* wt = malloc(sizeof(WorkTree));
	wt->tab = malloc(SIZE_TREE * sizeof(WorkFile));
	wt->size = SIZE_TREE;
	wt->n = 0;
	
	return wt;
}

int inWorkTree(WorkTree* wt, char* name) {
	int n = 0;
	while(n < wt->n) {
			if(strcmp(name, wt->tab[n].name) == 0) {
				return n;
			}
		n++;
	}
	return -1;
}

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode){
	if(inWorkTree(wt, name) != -1) {
		printf("Erreur: le fichier %s est déjà présent dans l'arbre.\n", name);
		return -1;
	}
	
	if(wt->n >= wt->size) {
		printf("Erreur: l'arbre est plein.\n");
		return -1;
	}
	
	wt->tab[wt->n].name = strdup(name);
	wt->tab[wt->n].mode = mode;
	if (hash != NULL) {
		wt->tab[wt->n].hash = strdup(hash);
	} else {
		wt->tab[wt->n].hash = NULL;
	}
	printf("Le fichier %s a été ajouté avec succès.\n", name);
	wt->n++;
	return 1;
}

char* wtts(WorkTree* wt) {
	char* wtts = (char*) malloc(1 + wt->n * 2006 * sizeof(char));
	for(int i = 0; i < wt->n; i++) {
		char* temp = wfts(wt->tab + i);
		strcat(wtts, temp);
		free(temp);
	}
	
	return wtts;
}

WorkTree* stwt(char* ch) {;
	WorkTree* wt = initWorkTree();
	char* buffer = (char*) malloc(2006 * sizeof(char));
	sscanf(ch, "%[^\n]", buffer);
	while(strchr(ch, '\n')) {
		char name[1000];
		char hash[1000];
		int mode;
		sscanf(ch, "%s\t%s\t%d\n", name, hash, &mode);
		appendWorkTree(wt, name, hash, mode);
		ch = strcpy(ch, ch + strlen(buffer) + 1);
		sscanf(ch, "%[^\n]", buffer);
	}
	free(buffer);
	return wt;
	
}

int wttf(WorkTree* wt, char* file) {
	FILE* ftree = fopen(file, "w");
	if (!ftree) {
		printf("Erreur lors de la création du fichier %s.\n", file);
		return -1;
	}
	
	fprintf(ftree, "%s", wtts(wt));
	
	fclose(ftree);
	
	return 1;
}

WorkTree* ftwt(char* file) {
	WorkTree* wt = initWorkTree();
	FILE* ftree = fopen(file, "r");
	if (!ftree) {
		printf("Erreur lors de l'ouverture du fichier %s.\n", file);
		free(wt);
		return NULL;
	}
	
	char* buffer = (char*) malloc(2006 * sizeof(char));
	
	
	while(fgets(buffer, 2006, ftree) != NULL) {
		char hash[1000];
		char name[1000];
		int mode;
		
		sscanf(buffer, "%s\t%s\t%d\n", name, hash, &mode);
		appendWorkTree(wt, name, hash, mode);
		
	}
	
	free(buffer);
	fclose(ftree);
	
	return wt;
}

char* blobWorkTree(WorkTree* wt) {
	char fname[14] = "/tmp/wtXXXXXX";
	mkstemp(fname);
	wttf(wt, fname);
	char* hash = sha256file(fname);
	char* wtbf = hashToFile(hash);
	strcat(wtbf, ".t");
	cp(wtbf, fname);
	return hash;
}

char* saveWorkTree(WorkTree* wt, char* path) {
	for (int i = 0; i < wt->n; i++) {
		char* absPath = concat_paths(path, wt->tab[i].name);
		if (isFile(absPath)) {
			blobFile(absPath);
			wt->tab[i].hash = sha256file(absPath);
			wt->tab[i].mode = getChmod(absPath);
		} else {
			WorkTree* nwt = initWorkTree();
			List* l = listdir(absPath);
			for (Cell* c_p = *l; c_p != NULL; c_p = c_p->next) {
				if (c_p->data[0] != '.') {
					appendWorkTree(nwt, c_p->data, NULL, 0);
				}
			}
			wt->tab[i].hash = saveWorkTree(nwt, absPath);
			wt->tab[i].mode = getChmod(absPath);
		}
	}
	return blobWorkTree(wt);
}
