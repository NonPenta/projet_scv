#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gitlib.h"
#include "reflib.h"

void initRefs() {
	if (!file_exists(".refs")) {
		system("mkdir .refs");
		system("touch .refs/master");
		system("touch .refs/HEAD");
	}
}

void createUpdateRef(char* ref_name, char* hash) {
	char* buffer = malloc((strlen(ref_name) + strlen(hash) + 15) * sizeof(char));
	sprintf(buffer, "echo %s > .refs/%s", hash, ref_name);
	system(buffer);
}

void deleteRef(char* ref_name){
	char* name_file = ".refs/";
	strcat(name_file, ref_name);
	if (!file_exists(name_file)) {
		printf("Erreur: référence %s inconnue.\n", ref_name);
	} else {
		char* buffer_sys = malloc((strlen(name_file) + 4) * sizeof(char));
		sprintf(buffer_sys, "rm %s", name_file);
		system(buffer_sys);
		
	}
}

char* getRef(char* refname) {
	char* name_file = ".refs/";
	strcat(name_file, refname);
	if (!file_exists(name_file)) {
		printf("Erreur: la référence %s n'existe pas.\n", refname);
		return NULL;
	}
	
	FILE* file = fopen(name_file, "r");
	
	if(!file) {
		printf("Erreur lors de l'ouverture du fichier");
		return NULL;
	}
	
	char* hash = malloc(sizeof(char)*1028);
	fgets(hash, 1028, file);
	return hash;
	
}