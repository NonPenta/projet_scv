#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "workfile.h"

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
	char* wtts = (char*) malloc(wt->n * 2006 * sizeof(char));
	for(int i = 0; i < wt->n; i++) {
		char* temp = wfts(wt->tab + i);
		strcat(wtts, temp);
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
	
	fclose(ftree);
	
	return wt;
}

int main() {
	
	WorkFile* wf = createWorkFile("essai");
	wf->hash = strdup("Hash_fictif");
	printf("%s\n", wfts(wf));
	WorkFile* wf2 = stwf("Nom_du_deuxième_worfile\thashnul\t777");
	printf("%s\n", wfts(wf2));
	printf("------\n");
	printf("Init WorkTree\n");
	WorkTree* wt = initWorkTree();
	appendWorkTree(wt, "nom1", "blabla", 777);
	appendWorkTree(wt, "nom2", "blabla", 777);
	appendWorkTree(wt, "nom1", "blablabla", 777);
	appendWorkTree(wt, "nom3", "blabla", 888);
	appendWorkTree(wt, "nom4", "blablabla", 345);
	printf("------\n");
	printf("%s\n", wtts(wt));
	WorkTree* wt2 = stwt(wtts(wt));
	wttf(wt2, "fichier_test");
	printf("------\n");
	WorkTree* wt3 = ftwt("fichier_test");
	printf("%s\n", wtts(wt3));
	
	
	
	return 0;
};