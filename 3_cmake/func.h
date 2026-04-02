#ifndef FUNK_H
#define FUNK_H
#include "structures.h"


void print_table(Table* table);
int erase(Table* table, unsigned int key, int release, char mode);
int insert(Table* table, unsigned int key, const char* info);
Table* find_individual(Table* table, unsigned int key, int release);
void free_table(Table* table);
int find(Table* table, unsigned int key, int release);
int init(Table* table, int msize);

#endif
