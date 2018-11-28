#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _LISTA_SIMPLU_INLANTUITA_
#define _LISTA_SIMPLU_INLANTUITA_

typedef struct Cell
{
	char *info;
	struct Cell *next;
} Cell, *List;

Cell *Create_Cell(char *value);
void append(List list, Cell *cell);
void Display_List(List list);

#endif

