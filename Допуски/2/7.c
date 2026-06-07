#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int push(ListQueue* queue, int value){
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if(new_node == NULL){
        return 1;
    }
    new_node -> value = value;
    new_node -> next = NULL;
    if(queue -> front == NULL){
        queue -> front = new_node;
        queue -> rear = new_node;
    }
    else{
        queue -> rear -> next = new_node;
        queue -> rear = new_node;
    }
    queue -> size++;
    return 0;
}