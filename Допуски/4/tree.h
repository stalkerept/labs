#ifndef TREE_H
#define TREE_H

typedef struct Node {
    char* key;
    char* info;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct Tree {
    Node* root;
} Tree;

#endif