#ifndef FUNC_H
#define FUNC_H
#include "structures.h"

char* insert(Tree* tree, const char* key, const char* info);
Node* find(Tree* tree, const char* key);
Node* find_ex(Node* node, char mode);
Node* find_special(Tree* tree, const char* key);
char delete(Tree* tree, const char* key);
void free_tree(Tree* tree);
Stack* stack_init(unsigned int capasity);
Tree* tree_init(void);

#endif