#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include <string.h>

MinHeap* init_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(sizeof(HeapNode) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode t = *a; *a = *b; *b = t;
    return;
}

void heap_push(MinHeap* heap, int vertex, int dist) {
    if (heap->size >= heap->capacity) return;
    int i = heap->size++;
    heap->data[i].vertex = vertex;
    heap->data[i].dist = dist;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->data[parent].dist > heap->data[i].dist) {
            swap(&heap->data[parent], &heap->data[i]);
            i = parent;
        } else break;
    }
}

int heap_pop(MinHeap* heap, int* out_vertex, int* out_dist) {
    if (heap->size == 0) return 0;
    *out_vertex = heap->data[0].vertex;
    *out_dist = heap->data[0].dist;
    heap->data[0] = heap->data[--heap->size];
    int i = 0;
    while (1) {
        int smallest = i;
        int left = 2*i + 1;
        int right = 2*i + 2;
        if (left < heap->size && heap->data[left].dist < heap->data[smallest].dist)
            smallest = left;
        if (right < heap->size && heap->data[right].dist < heap->data[smallest].dist)
            smallest = right;
        if (smallest != i) {
            swap(&heap->data[i], &heap->data[smallest]);
            i = smallest;
        } else break;
    }
    return 1;
}

void free_heap(MinHeap* heap) {
    free(heap->data);
    free(heap);
    return;
}