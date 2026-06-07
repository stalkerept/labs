#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Node {
    int release;
    char* info;
    struct Node *next;
};

typedef struct KeySpace {
    unsigned int key;
    struct Node *node;
}KeySpace;

typedef struct Table
{
    KeySpace* ks;
    int msize;
    int size;
}Table;


#endif