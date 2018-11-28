#include <stdio.h>
#include "List.h"

#ifndef _HASH_
#define _HASH_

typedef struct Hash
{
	List *vector;
	int length;
} Hash, *Hashmap;

int get_list_length(List l);
void add_elem(Hashmap hash_map, Cell *node);
void remove_elem(Hashmap hash_map, char *info);
void find_elem(Hashmap hash_map, char *info, FILE *output_file);
void print_bucket(Hashmap hash_map, int index_bucket, FILE *output_file);
void print_hash(Hashmap hash_map, FILE *output_file);
void clear_hash(Hashmap hash_map);
void resize(Hashmap new_map, Hashmap hash_map);
Hashmap HashmapAlloc(int length);
void HashmapDealloc(Hashmap hash_map);

#endif