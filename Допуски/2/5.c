#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int pop(VectorQueue* queue, int* result){
    if(queue -> size == 0) return 1;
    *result = queue -> data[queue -> front];
    queue -> front = (queue -> front + 1)%(queue->capacity);
    queue -> size--;
    return 0;
}