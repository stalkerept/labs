#ifndef HEAP_H
#define HEAP_H
#include "structures.h"

MinHeap* init_heap(int capacity);
void swap(HeapNode* a, HeapNode* b);
void heap_push(MinHeap* heap, int vertex, int dist);
int heap_pop(MinHeap* heap, int* out_vertex, int* out_dist);
void free_heap(MinHeap* heap);
#endif