#ifndef FUNC_HASH_H
#define FUNC_HASH_H
#include "structures.h"

unsigned long hash_string(unsigned char *str);
int find(Table* table, char* coords);
char insert(Table* table, int key, char* coords, unsigned long (*hash)(unsigned char*));
char delete(Table* table, const char* coords, unsigned long (*hash)(unsigned char*));
Table* init_table(int capasity);
void free_table(Table* table);
#endif

