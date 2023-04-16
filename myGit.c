#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "clist.h"
#include "reflib.h"
#include "gitlib.h"
#include "workfile.h"

int main(int argc, char** argv) {
	if(strcmp(argv[1], "init") == 0) {
		initRefs();
	}
	
	if(strcmp(argv[1], "list-refs") == 0) {
		if(!file_exists(".refs") {
			printf("Aucune référence présente");
			return 0;
		}
		   
		printf("Références :\n");
		List* L = listdir(".refs");
		for (Cell* ptr = *L; ptr != NULL; ptr = ptr->next) {
			if (ptr->data[0] == '.') {
				continue;
			}
			char* reference = getRef(ptr->data);
			printf("%s \t %s\n", ptr->data, content);
		}
	}
	
	if(strcmp(argv[1], "create-ref") == 0) {
		createUpdateRef(argv[2], argv[3]);
	}
		   
	if(strcmp(argv[1], "delete-ref") == 0) {
		deleteRef(argv[2]);
	}
		   
	if(strcmp(argv[1], "add") == 0) {
		for(int i = 2; i < argc; i++) {
			myGitAdd(argv[i]);
		}
	}
		   
	if(strcmp(argv[1], "list-add") == 0) {
		if(!file_exists(".add"); {
			printf("Aucun fichier dans la zone de préparation");
			return 0;
		}
		
		printf("Fichiers dans la zone de préparation: \n");
		WorkTree* wt = ftwt(".add");
		printf("%s", wtts(wt));
	}
		   
	if(strcmp(argv[1], "clear-add") == 0) {
		system("rm .add");
	}
		   
	if(strcmp(argv[1], "commit") == 0) {
		if(strcmp(argv[3], "-m") == 0) {
			myGitCommit(argv[2], argv[4]);
		} else {
			myGitCommit(argv[2], NULL);
		}
	}
		   
	return 0;
		   
	
}