#include "List.h"
#include "HashFunc.h"

#ifndef LIN_UTILS_H_
#define LIN_UTILS_H_	1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFF_SIZE 20000

/* useful macro for handling error codes */
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0)

#endif

int convert(char *length)
{
	int i = 0;
	int conv = 0;
	while (length[i] != '\0') {
		conv = conv * 10 + (length[i] - '0');
		i++;
	}

	return conv;
}

int check_if_number(char *str)
{
	int length = strlen(str);
	int i;

	for (i = 0; i < length; i++) {
		if (!(isdigit(str[i])))
			return 0;
	}

	return 1;
}

List ListAlloc(int length)
{
	List l = malloc(sizeof(Cell) * length);
	return l;
}

int read_input(Hashmap hash_map, FILE *input)
{
	char *buffer = malloc(sizeof(char) * BUFF_SIZE);
	if (buffer == NULL)
		return -1;

	char *s = malloc(sizeof(char) * 2);
	s[0] = ' ';
	s[1] = '\n';
	char *token, *aux_token;
	int rsz_len;
	int i_bucket;
	while (fgets(buffer, BUFF_SIZE, input) != NULL) {
		if (strcmp(buffer, "\n") == 0)
			continue;
		token = strtok(buffer, s);
		if (token == NULL) {
			return -1;
		} else if (strcmp(token, "add") == 0) {
			token = strtok(NULL, s);
			Cell *aux = Create_Cell(token);
			add_elem(hash_map, aux);
		} else if (strcmp(token, "remove") == 0) {
			token = strtok(NULL, s);
			remove_elem(hash_map, token);
		} else if (strcmp(token, "find") == 0) {
			token = strtok(NULL, s);
			aux_token = strtok(NULL, s);
			if (aux_token == NULL)
				find_elem(hash_map, token, NULL);
			if (aux_token != NULL) {
				FILE *output_file;
				output_file = fopen(aux_token, "a");
				find_elem(hash_map, token, output_file);
				fclose(output_file);
			}
		} else if (strcmp(token, "print_bucket") == 0) {
			token = strtok(NULL, s);
			if (check_if_number(token) == 1)
				i_bucket = convert(token);
			else
				return -1;

			aux_token = strtok(NULL, s);
			if (aux_token == NULL) {
				print_bucket(hash_map, i_bucket, NULL);
				printf("\n");
			}

			if (aux_token != NULL) {
				FILE *output_file = fopen(aux_token, "a");
				print_bucket(hash_map, i_bucket, output_file);
				fprintf(output_file, "\n");
				fclose(output_file);
			}
		} else if (strcmp(token, "clear") == 0) {
			clear_hash(hash_map);
		} else if (strcmp(token, "print") == 0) {
			token = strtok(NULL, s);
			if (token == NULL) {
				print_hash(hash_map, NULL);
				printf("\n");
			}
			if (token != NULL) {
				FILE *output_file = fopen(token, "a");
				print_hash(hash_map, output_file);
				fprintf(output_file, "\n");
				fclose(output_file);
			}
		} else if (strcmp(token, "resize") == 0) {
			Hashmap new_hash = malloc(sizeof(Hash));
			if (new_hash == NULL)
				return -1;

			token = strtok(NULL, s);
			DIE(check_if_number(token) == 1, "resize");

			if (strcmp(token, "double") == 0)
				rsz_len = 2 * hash_map->length;
			if (strcmp(token, "halve") == 0)
				rsz_len = hash_map->length / 2;

			new_hash->vector = malloc(sizeof(List) * rsz_len);
			if (new_hash->vector == NULL) {
				free(new_hash);
				return -1;
			}

			new_hash->length = rsz_len;

			resize(new_hash, hash_map);
			hash_map = new_hash;
			hash_map->length = new_hash->length;
		} else
			return -1;
	}

	free(buffer);
	return 0;
}

int main(int argc, char **argv)
{
	DIE(argc == 1, "Prea putini parametri!");

	int hash_length = 0;
	int read_input_rc = 0;
	int i;

	Hashmap hash_map = NULL;
	hash_length = convert(argv[1]);
	hash_map = HashmapAlloc(hash_length);

	if (hash_map == NULL)
		return -1;

	if (argc == 2) {
		DIE(hash_length < 0, "Eroare!");
		DIE(check_if_number(argv[1]) == 0, "Eroare!");
		read_input_rc = read_input(hash_map, stdin);
		DIE(read_input_rc < 0, "Eroare!");
	} else if (argc > 2) {
		FILE *fd;
		DIE(hash_length < 0, "Eroare!");
		DIE(check_if_number(argv[1]) == 0, "Eroare!");
		for (i = 2; i < argc; i++) {
			fd = fopen(argv[i], "r");
			read_input_rc = read_input(hash_map, fd);
			DIE(read_input_rc < 0, "Eroare!");
		}
	}

	HashmapDealloc(hash_map);
	free(hash_map);
}
