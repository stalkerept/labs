#include <stdlib.h>
#include "structures.h"
#include <stdio.h>


int push(Stack* stack, int value){
    if(stack -> top + 1 >= stack -> size) return 1;
    stack -> top++;
    stack-> data[stack -> top] = value;
    return 0;
}