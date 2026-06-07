#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct 
{
    int data;
    char* coords;
}Node;


typedef struct{
    Node* data;
    int len;
    int capasity;
}Table;

typedef struct Leaf
{
    struct Leaf* right;
    struct Leaf* left;
    struct Leaf* parent;
    int key;
    char* coords;
}Leaf;

typedef struct
{
    Leaf* root;
    int count;
}Tree;

typedef struct Stack
{
    int top;
    int capacity;
    Leaf** array;
}Stack;


#endif