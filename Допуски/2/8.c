#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int pop(ListQueue* queue, int* result){
    if(queue -> front == NULL){
        return 1;
    }
    QueueNode* remember = queue -> front;
    *result = remember ->value;
    queue -> front = remember->next;
    if(queue -> front == NULL){
        queue -> rear = NULL;
    }
    free(remember);
    queue -> size--;
    return 0;
}