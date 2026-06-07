#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int pop(Stack* stack, int* result){
    if(stack -> top < 0) return 1;
    *result = stack-> data[stack -> top];
    stack -> top--;
    return 0;
}