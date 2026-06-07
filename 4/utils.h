#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "func.h"

void print_RNL(Tree* tree, const char* min_key, const char* max_key);
void print_tree_visual(Node* node, int space);
void generate_dot_node(FILE* file, const Node* node);
int generate_graphviz(Tree* tree, const char* filename);
char* enough_space(char* str);
int parse_add_input(char* input, char** key, char** value);
int load_tree_from_file(Tree* tree, const char* filename);
#endif