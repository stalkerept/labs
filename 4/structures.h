#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Node
{
    char* key;
    char* info;
    struct Node* right;
    struct Node* left;
    struct Node* parent;
}Node;

typedef struct Tree
{
    Node* root;
    int count;
} Tree;


typedef struct Stack
{
    int top;
    int capasity;
    Node** array;
}Stack;

typedef struct {
    int size;
    double insert_time_us;
    double delete_time_us;
    double find_time_us;
    double special_time_us;
    double traverse_time_us;
} BenchmarkPoint;


#endif