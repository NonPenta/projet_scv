#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "clist.h"

List* initList() {
	List* l = malloc(sizeof(List));
	*l = NULL;
	return l;
}
Cell* buildCell(char* ch) {
	Cell* c = malloc(sizeof(Cell));
	c->data = strdup(ch);
	c->next = NULL;
	return c;
}
void freeCell(Cell* c) {
	free(c->data);
	free(c);
}
void freeList(List* l) {
	Cell* c = *l;
	while (c) {
		Cell* tf = c;
		c = c->next;
		freeCell(tf);
	}
	free(l);
}

void insertFirst(List* l, Cell* c) {
	c->next = *l;
	*l = c;
}
Cell* listGet(List* l, int i) {
	Cell* c = *l; // elem 0
	for (int it = 0; it < i; it++) {
		c = c->next;
		if (c == NULL) { return c; }
	}

	return c;
}
Cell* searchList(List* l, char* str) {
	Cell* c = *l;
	while (strncmp(str, c->data, strlen(str)) != 0 && c) {
		c = c->next;
	}

	return c;
}

char* ctos(Cell* c) {
	return strdup(c->data);
}
char* ltos(List* l) {
	Cell* c = *l;
	char* s = malloc(strlen(c->data));
	sprintf(s, "%s", c->data);
	while (c->next) {
		c = c->next;
		s = realloc(s, strlen(s) + 1 + strlen(c->data));
		sprintf(s, "%s|%s", s, c->data);
	}

	return s;
}
List* stol(char* s) {
	List* l = initList();

	unsigned long int i = 0;
	while (i < strlen(s)) {
		int cl = strcspn(s+i, "|");
		char* c = calloc(sizeof(char), cl);
		strncpy(c, s+i, cl);

		Cell* p_c = buildCell(c);
		insertFirst(l, p_c);

		i = i + cl + 1;
	}

	return l;
}

void ltof(List* l, char* path) {
	Cell* c = *l;
	FILE* f = fopen(path, "w");
	
	while (c) {
		fprintf(f, "%s\n", c->data);
		c = c->next;
	}

	fclose(f);
}
List* ftol(char* path) {
	List* l = initList();
	FILE* f = fopen(path, "r");
	size_t i = 255;
	char* buf = malloc(i * sizeof(char));
	while (getline(&buf, &i, f)) {
		Cell* c = buildCell(buf);
		insertFirst(l, c);
	}

	return l;
}

List* filterList(List* L, char* pattern) {
	List* filtre = initList();
	Cell* cell = *L;
	while(cell != NULL) {
		char* c = strdup(cell->data);
		c[strlen(pattern)] = '\0';
		if(strcmp(c, pattern) == 0) {
			insertFirst(filtre, buildCell(cell->data));
		}
		free(c);
	}
	return filtre;
}

int listSize(List* list) {
	int count = 0;
	Cell* cell = *list;
	while(cell != NULL) {
		count++;
		cell = cell->next;
	}
	return count;
}
