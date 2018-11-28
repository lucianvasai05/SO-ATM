#include "hash.h"
#include "HashFunc.h"
#include "List.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_list_length(List l)
{
	List c = l;
	int counter = 0;
	if (l != NULL) {
		counter = 1;
		while (c->next != NULL) {
			counter++;
			c = c->next;
		}
	}
	return counter;
}

void add_elem(Hashmap hash_map, Cell* cell)
{
	unsigned int key = hash(cell->info, hash_map->length);
	List aux = hash_map->vector[key];
	List head = aux;
	
	if (aux == NULL) {
		aux = cell;
		hash_map->vector[key] = aux;
		return;
	}

	if (get_list_length(aux) > 1) {
		do {
			if (strcmp(aux->info, cell->info) == 0)
				return;
			aux = aux->next;
		} while(aux->next != NULL);
	}

	if (strcmp(aux->info, cell->info) == 0)
		return;

	aux->next = cell;
	hash_map->vector[key] = head;
}

void remove_elem(Hashmap hash_map, char* info)
{
	unsigned int key = hash(info, hash_map->length);
	List aux = hash_map->vector[key];
	List head = aux;
	List prev = NULL;
	Cell *temp = malloc(sizeof(Cell));
	int found = 0;

	if (aux == NULL)
		return;

	if (!temp)
		return;

	do {
		if (strcmp(aux->info, info) == 0) {
			temp = aux;
			found = 1;
			break;
		}
		prev = aux;
		aux = aux->next;
	} while(aux->next != NULL);

	if (strcmp(aux->info, info) == 0) {
		temp = aux;
		found = 1;
	}
	
	if (found == 0) 
		return;
	
	if (get_list_length(head) == 1 && found == 1) {
		head->next = NULL;
		head = NULL;
		free(head);

		hash_map->vector[key] = head;
		return;
	} else {
		Cell *next = temp->next;
		if (next == NULL) {
			prev->next = NULL;
			return;
		}
		free(temp);
		if (prev == NULL) {
			head = next;
		} else {
			prev->next = next;
		}
	}

	hash_map->vector[key] = head;
}

void find_elem(Hashmap hash_map, char *info, FILE *output_file)
{
	unsigned int key = hash(info, hash_map->length);

	List aux = hash_map->vector[key];
	if (aux == NULL && output_file == NULL) {
		printf("\nFalse\n\n");
		return;
	} else if (aux == NULL && output_file != NULL) {
		fprintf(output_file, "\n%s\n\n", "False");
		return;
	}

	do {
		if (strcmp(aux->info, info) == 0) {
			if (output_file == NULL)
				printf("\nTrue\n\n");
			else
				fprintf(output_file, "\nTrue\n\n");
			return;
		}
		aux = aux->next;
	} while(aux->next != NULL);

	if (strcmp(aux->info, info) == 0) {
		if (output_file == NULL)
			printf("\nTrue\n\n");
		else
			fprintf(output_file, "\nTrue\n\n");
		return;
	}

	if (output_file == NULL)
		printf("\nFalse\n\n");
	else
		fprintf(output_file, "\n%s\n\n", "False");
}

void print_bucket(Hashmap hash_map, int index_bucket, FILE *output_file)
{
	List l = hash_map->vector[index_bucket];
	int i;
	if (output_file == NULL) {
		Display_List(l);
	} else {
		if (l == NULL) {
			return;
		} else {
			i = 1;
			while (l->next != NULL) {
				fprintf(output_file, "%s ", l->info);
				i++;
				l = l->next;
			}
			fprintf(output_file, "%s", l->info);
		}
	}
}

void print_hash(Hashmap hash_map, FILE *output_file)
{
	int i;
	for (i = 0; i < hash_map->length; i++) {
		print_bucket(hash_map, i, output_file);
		if (hash_map->vector[i] != NULL)
			if (i != hash_map->length - 1)
				if (output_file != NULL)
					fprintf(output_file, "\n");
	}
}

void clear_hash(Hashmap hash_map)
{
	int i;

	List curr, next;
	for (i = 0; i < hash_map->length; i++) {
		curr = hash_map->vector[i];
		while (curr != NULL) {
			next = curr->next;
			free(curr);
			curr = next;
		}
		hash_map->vector[i] = NULL;
		curr = NULL;
	}
}

Hashmap HashmapAlloc(int length)
{
	Hashmap h = malloc(sizeof(Hash));
	if (h == NULL)
		return NULL;

	h->vector = malloc(sizeof(List) * length);

	if (h->vector == NULL) {
		free(h);
		return NULL;
	}

	int i;
	h->length = length;

	for (i = 0; i < length; i++)
		h->vector[i] = NULL;

	return h;
}

void HashmapDealloc(Hashmap hash_map)
{
	int i;
	List node, temp;
	for (i = 0; i < hash_map->length; i++) {
		node = hash_map->vector[i];
		while (node != NULL) {
			temp = node;
			node = node->next;
			if (temp->info != NULL)
				free(temp->info);
			free(temp);
		}
		free(node);
	}
}

void resize(Hashmap new_hash, Hashmap hash_map)
{
	int i;
	Cell *c;

	for (i = 0; i < hash_map->length; i++) {
		List aux = hash_map->vector[i];
		if (aux != NULL) {
			c = malloc(sizeof(Cell));
			while (aux->next != NULL) {
				c = Create_Cell(aux->info);
				add_elem(new_hash, c);
				aux = aux->next;
			}

			c = Create_Cell(aux->info);
			add_elem(new_hash, c);
		}
	}
}
