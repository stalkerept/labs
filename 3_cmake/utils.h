#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include "structures.h"

int read_unsigned_int(unsigned int *value);
int read_string(char *buffer, size_t max_len);
void clear_input();
void show_menu();
int import_from_file(Table *table, const char *filename);
#endif