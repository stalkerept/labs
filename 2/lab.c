#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "func.h"
#include <string.h>


int main(void){
    Error error;
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if(!queue){
        error.error_char = 1;
        error.number = 1;
        check_error(error, queue);
        return 1;
    }
    queue -> top = NULL;
    queue -> bottom = NULL;
    int count = 0;
    printf("Enter the number of items in the queue:");
    error = read_int(&count);
    if(error.error_char != 0){
        check_error(error, queue);
        return 1;
    }
    int n = 0;
    while(n < count){
        error.error_char = 0;
        Info* new_man = (Info*)malloc(sizeof(Info));
        if(!new_man){
            error.error_char = 1;
            error.number = 1;
            check_error(error, queue);
            return 1;
        }
        printf("%d element:", n+1);
        error = read_info(new_man);
        if (error.error_char != 0) {
            free(new_man);
            check_error(error, queue);
            return 1;
        }
        error = push(queue, *new_man);
        free(new_man);
        if(error.error_char != 0){
            check_error(error, queue);
            return 1;
        }
        n++;
    }
    print_queue(queue);
    clear_queue(queue);
    free(queue);
    return 0;
}