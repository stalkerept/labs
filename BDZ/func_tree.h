#ifndef FUNC_TREE_H
#define FUNC_TREE_H
#include "structures.h"

Leaf* splay(Leaf* root, const char* coords);
Leaf* find(Tree* tree, const char* coords);
Leaf* find_ex(Leaf* node, char mode);
char insert(Tree* tree, int key, const char* coords);
Tree* tree_init(void);
Stack* stack_init(unsigned int capacity);
void free_tree(Tree* tree);
char delete(Tree* tree, const char* coords);

#endif