#include <stdio.h>
#include <string.h>
#include "gitlib.h"
#include "hashlib.h"

int main() {
	// .
	int i = file_exists("clist.h");
	printf("%d\n", i);
	cp("clist.h_copy", "clist.h");
	printf("%s", hashToPath(sha256file("clist.h")));
	blobFile("clist.h");
	return 0;
}
