#include <stdio.h>
#include <string.h>
#include "gitlib.h"
#include "hashlib.h"
#include "workfile.h"

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
	// .
	int i = file_exists("clist.h");
	printf("%d\n", i);
	cp("clist.h_copy", "clist.h");
	printf("%s", hashToPath(sha256file("clist.h")));
	blobFile("clist.h");
	return 0;
}
