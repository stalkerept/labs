#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum{
    WHITE,
    GRAY,
    BLACK
}Color;

typedef struct Vertex{
    char* name;
    Color color;
    char deleted;
}Vertex;

typedef struct AdjNode {
    int dest;              
    int weight;
    struct AdjNode* next;
} AdjNode;

typedef struct Graph{
    Vertex* vertices;
    int len;
    int capacity;
    AdjNode** heads;
}Graph;

typedef struct {
    int vertex;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;


#endif