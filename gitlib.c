#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "hashlib.h"
#include "gitlib.h"
#include "clist.h"
#include "reflib.h"
#include "workfile.h"
#include "commit.h"

List* listdir(char* root_dir) {
	List* l = initList();
	DIR* dp = opendir(root_dir);
	struct dirent* ep;
	if (dp == NULL) { return l; }
	while ((ep = readdir(dp))) {
		Cell* c = buildCell(ep->d_name);
		insertFirst(l, c);
	}

	closedir(dp);
	return l;
}

int file_exists(char* file) {
	struct stat buffer;
	return (stat(file, &buffer) == 0);
}

void cp(char* to, char* from) {
	if (file_exists(from)) {
		FILE* ffrom = fopen(from, "r");
		FILE* fto = fopen(to, "w");

		char line[128];
		while (fgets(line, 128, ffrom)) {
			fputs(line, fto);
		}
	}
}

void blobFile(char* file) {
	char* hash = sha256file(file);

	char* dir = strdup(hash); dir[2] = '\0';
	char* path = hashToPath(hash);

	if (!file_exists(dir)) {
		char command[9];
		sprintf(command, "mkdir %c%c", path[0], path[1]);
		system(command);
	}

	free(dir);
	cp(path, file);
}

int getChmod(const char *path) {
	struct stat ret;
	if (stat(path, &ret) == -1) {
		return -1;
	}
  
	return	100*(((ret.st_mode & S_IRUSR) + (ret.st_mode & S_IWUSR) + (ret.st_mode & S_IXUSR))/64) +         
				10*(((ret.st_mode & S_IRGRP) + (ret.st_mode & S_IWGRP) + (ret.st_mode & S_IXGRP)))/8 +
				(ret.st_mode & S_IROTH) + (ret.st_mode & S_IWOTH) + (ret.st_mode & S_IXOTH); 
}

void setMode(int mode, char* path) {
	char buff[100];
	sprintf(buff, "chmod %d %s", mode, path) ;
	system(buff);
}

char* concat_paths(char* p1, char* p2) {
	char* res = malloc(strlen(p1) + strlen(p2) + 2);
	if (res == NULL) { printf("Unable to allocate memory (for god knows what reason)"); return NULL; }
	sprintf(res, "%s/%s", p1, p2);
	return res;
}

int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void createFile(char* file) {
	char* sysbuff = malloc((6 + strlen(file)) * sizeof(char));
	sprintf(sysbuff, "touch %s", file);
	system(sysbuff);
}

void myGitAdd(char* file_or_folder) {
	WorkTree* wt;
	
	if(!file_exists(".add")) {
		createFile(".add");
		wt = initWorkTree();
	} else {
		wt = ftwt(".add");
	}
	
	if (file_exists(file_or_folder)){
		appendWorkTree(wt, file_or_folder, 0, 777);
		wttf(wt, ".add");
	} else {
		printf("Erreur: Le fichier ou le dossier %s n'existe pas.\n", file_or_folder);
	}
}

void myGitCommit(char* branch_name, char* message) {
	printf(".\n");
	if(!file_exists(".refs")) {
		printf("Erreur: Références non initialisées.\n");
		return;
	}
	
	printf(".\n");
	
	char* file_name = malloc(1000 * sizeof(char));
	
	printf(".\n");
	
	sprintf(file_name, ".refs/%s", branch_name);
	
	printf(".\n");
	
	if(!file_exists(file_name)) {
		printf("Erreur: Branche inexistante.\n");
		return;
	}
	
	printf(".\n");
	
	char* branchref = getRef(branch_name);
	char* headref = getRef("HEAD");
	
	if(strcmp(branchref, headref) != 0) {
		printf("Erreur: HEAD doit pointer sur le dernier commit de la branche.\n");
		return;
	}
	
	WorkTree* wt = ftwt(".add");
	system("rm .add");
	
	char* hashwt = saveWorkTree(wt, ".");
	Commit* c = createCommit(hashwt);
	if(strlen(branchref) != 0) {
		commitSet(c, "predecessor", branchref);
	}
	if (message != NULL) {
		commitSet(c, "message", message);
	}
	
	char* hashc = blobCommit(c);
	createUpdateRef(branch_name, hashc);
	createUpdateRef("HEAD", hashc);
}

void myGitCheckoutBranch(char* branch) {
	FILE* f = fopen(".current_branch", "w");
	fprintf(f, "%s", branch);
	fclose(f);
	
	char* hash_commit = getRef(branch);
	createUpdateRef("HEAD", hash_commit);
	restoreCommit(hash_commit);
}