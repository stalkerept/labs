#ifndef FUNC_HASH_H
#define FUNC_HASH_H
#include "structures.h"
#include <stdint.h>

int find(Table* table, char* coords);
char insert(Table* table, int key, char* coords);
char delete(Table* table, const char* coords);
Table* init_table(int capasity, uint64_t (*hash)(const char*));
void free_table(Table* table);

#endif
