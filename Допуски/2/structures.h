#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Stack {
    int size;     // максимальная вместимость (ёмкость)
    int top;      // индекс вершины стека (последнего добавленного элемента)
    int *data;    // указатель на массив элементов
} Stack;

typedef struct StackNode {
    int value;
    struct StackNode *next;
} StackNode;

typedef struct ListStack {
    StackNode *top;    // указатель на вершину
} ListStack;


typedef struct VectorQueue {
    int* data;
    int front;
    int rear;
    int size;
    int capacity;
} VectorQueue;

typedef struct QueueNode {
    int value;
    struct QueueNode* next;
} QueueNode;

typedef struct ListQueue {
    QueueNode* front;  // указатель на начало (откуда извлекаем)
    QueueNode* rear;   // указатель на конец (куда добавляем)
    int size;          // количество элементов
} ListQueue;

#endif