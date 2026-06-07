#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int push(ListStack* stack, int value){
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if(!new_node){
        return 1;
    }
    new_node -> value = value;
    new_node -> next = stack -> top;
    stack -> top = new_node;
    return 0;
}