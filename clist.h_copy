#ifndef CLIST
#define CLIST

typedef struct cell {
	char* data;
	struct cell* next;
} Cell;

typedef Cell* List;

List* initList();
Cell* buildCell(char* ch);
void freeCell(Cell* c);
void freeList(List* l);

void insertFirst(List* L, Cell* C);
Cell* listGet(List* l, int i);
Cell* searchList(List* l, char* str);

char* ctos(Cell* c);
char* ltos(List* l);
List* stol(char* s);

void ltof(List* l, char* path);
List* ftol(char* path);

#endif
