#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int pop(ListStack* stack, int* result){
    if(stack -> top == NULL){
        return 1;
    }
    StackNode* remember = stack -> top;
    *result = stack -> top;
    stack -> top = remember -> next;
    free(remember);
    return 0;
}