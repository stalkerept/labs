#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int push(VectorQueue* queue, int value){
    if(queue -> size >= queue -> capacity){
        return 1;
    }
    queue -> data[queue -> rear] = value;
    queue -> rear = (queue -> rear + 1)%queue -> capacity;
    queue -> size++;
    return 0;
}