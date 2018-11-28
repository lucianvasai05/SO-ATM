#include "List.h"
#include<stdio.h>
#include<stdlib.h>


Cell *Create_Cell(char *value)
{
	Cell *c = malloc(sizeof(Cell));
	c->info = strdup(value);
	c->next = NULL;
	return c;
}

void append(List l, Cell *c)
{
	Cell *last = l;

	if(l == NULL){
		l = c;
		return;
	}

	while(last->next != NULL){
		last = last->next;
	}

	last->next = c;
}

void Display_List(List l)
{
	if (l == NULL) {
		return;
	} else {
		while (l->next != NULL) {
			printf("%s ", l->info);
			l = l->next;
		}
		printf("%s\n", l->info);
	}
} 
