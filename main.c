#include <stdio.h>
#include <string.h>
#include "gitlib.h"
#include "hashlib.h"
#include "workfile.h"

int main() {
	
	printf("Init WorkTree\n");
	WorkTree* wt = initWorkTree();
	appendWorkTree(wt, "projet_scv", NULL, 0);
	saveWorkTree(wt, "..");
	printf("------\n");
	printf("%s\n", wtts(wt));
	WorkTree* wt2 = stwt(wtts(wt));
	wttf(wt2, "fichier_test");
	printf("------\n");
	WorkTree* wt3 = ftwt("fichier_test");
	printf("%s\n", wtts(wt3));
	
	
	
	return 0;
	// .
	int i = file_exists("clist.h");
	printf("%d\n", i);
	cp("clist.h_copy", "clist.h");
	printf("%s", hashToPath(sha256file("clist.h")));
	blobFile("clist.h");
	return 0;
}
