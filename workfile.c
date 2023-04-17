#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "workfile.h"
#include "hashlib.h"
#include "gitlib.h"
#include "clist.h"

WorkFile* createWorkFile(char* name) {
	WorkFile* wf = (WorkFile*) malloc(sizeof(WorkFile));
	wf->name = strdup(name);
	wf->hash = NULL;
	wf->mode = 0;

	return wf;
}

char* wfts(WorkFile* wf) {
	char* hash = NULL;
	if (wf->hash != NULL) {
		hash = strdup(wf->hash);
	} 
	char* wfts = (char*) malloc((6 + strlen(wf->name) + 64) * sizeof(char));
	sprintf(wfts, "%s\t%s\t%d\n", wf->name, wf->hash, wf->mode);
	free(hash);
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
	wtts[0] = '\0';
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
	wtbf = realloc(wtbf, 67);
	strcat(wtbf, ".t");
	cp(wtbf, fname);
	char rm[17] = "rm ";
	strcat(rm, fname);
	system(rm);
	return hash;
}

char* saveWorkTree(WorkTree* wt, char* path) {
	for (int i = 0; i < wt->n; i++) {
		char* absPath = concat_paths(path, wt->tab[i].name);
		printf("%s : isfile -> %d\n", absPath, isFile(absPath));
		if (isFile(absPath)) {
			printf("%d", isFile(absPath));
			blobFile(absPath);
			wt->tab[i].hash = sha256file(absPath);
			wt->tab[i].mode = getChmod(absPath);
		} else {
			printf("nwt side : %s", absPath);
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

int isWorkTree(char* path) {
	char* tmp_path = malloc(strlen(path) + 2);
	strcpy(tmp_path, path);
	return file_exists(strcat(tmp_path, ".t")) ? 1 : (file_exists(path) ? 0 : -1);
}

void restoreWorkTree(WorkTree* wt, char* path) {
	for (int i = 0; i < wt->n; i++) {
		char* dstPath = concat_paths(path, wt->tab[i].name);
		char* srcPath = hashToPath(wt->tab[i].hash);
		if (isWorkTree(srcPath) == 0) {
			cp(dstPath, srcPath);
			setMode(getChmod(srcPath), dstPath);
		} else if (isWorkTree(srcPath) == 1) {
			srcPath = realloc(srcPath, strlen(srcPath) + 2);
			strcat(srcPath, ".t");
			WorkTree* nwt = ftwt(srcPath);
			restoreWorkTree(nwt, srcPath);
			setMode(getChmod(srcPath), dstPath);
			free(nwt);
		}
	}
}

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts) {
	*conflicts = initList();
	WorkTree* wt3 = initWorkTree();
	for(int i =0; i < wt1->n; i++) {
		if(inWorkTree(wt2, wt1->tab[i].name) > -1){
			Cell* c = buildCell(wt1->tab[1].name);
			insertFirst(*conflicts, c);
		} else {
			appendWorkTree(wt3, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);
		}
	}
	
	for(int i = 0; i < wt2->n; i++) {
		if(inWorkTree(wt1, wt2->tab[i].name) == -1){
			appendWorkTree(wt3, wt2->tab[i].name, wt2->tab[i].hash, wt2->tab[i].mode);
		}
	}
	
	return wt3;
}
