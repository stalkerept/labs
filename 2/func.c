#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include <ctype.h>
#include <string.h>
#include <limits.h>

char is_clear(Queue* queue){
    if(queue -> top == NULL && queue -> bottom == NULL){
        return 1;
    }
    return 0;
}

Error push(Queue* queue, Info value){
    Error error = {0, 0};
    struct Person* new_person = (struct Person*)malloc(sizeof(struct Person));
    if(!new_person){
        error.error_char = 1;
        error.number = 1;
        return error;
    }
    new_person -> man = value;
    new_person -> next = NULL;
    if(is_clear(queue) == 1){
        queue -> bottom = new_person;
        queue -> top = new_person;
    }
    else if(value.j_a == 1){
        queue -> bottom -> next = new_person;
        queue -> bottom = new_person;
    }
    else{
        new_person -> next = queue -> top;
        queue -> top = new_person;
    }
    return error;
}

void print_queue(Queue* queue){
    struct Person* now = queue -> top;
    int count = 1;
    while(now!=NULL){
        now = now->next;
        count++;
    }
    now = queue -> top;
    printf("Queue:\n");
    while(now!=NULL){
        count--;
        printf("%d element: %hhd/%s/%d/%d\n", 
                  count, now->man.j_a, now->man.id, now->man.ta, now->man.ts);
        now = now->next;
    }
}

void clear_queue(Queue* queue){
    struct Person* now = queue -> top;
    while(now != NULL){
        struct Person *new = now -> next;
        free(now);
        now = new;
    }
    queue -> top = NULL;
    queue -> bottom = NULL;
}

char check_error(Error error, Queue* queue){
    if(error.error_char != 0){
        if(queue != NULL){
            clear_queue(queue);
            free(queue);
        }
        printf("Error: %hhd/%hhd\n", error.error_char, error.number);
        switch (error.number)
        {
        case 1:
            printf("\nMemory error\n");
            break;
        case 2:
            printf("\nInvalid input. Please enter in the format *string/number/number or string/number/number. In numbers use no more than 10 digits. Not use format */number/number\n");
            break;
        default:
            break;
        }
        return 1;
    }
    return 0;
}


Error read_info(Info* new_man) {
    Error error = {0, 0};
    char line[256];

    if (fgets(line, sizeof(line), stdin) == NULL) {
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    line[strcspn(line, "\n")] = '\0';
    char *token_start = line;
    if (line[0] == '*') {
        new_man->j_a = 1;
        token_start++;
    } else {
        new_man->j_a = 0;
    }
    char *id_token = NULL, *ta_token = NULL, *ts_token = NULL;
    int token_count = 0;
    char *token = strtok(token_start, "/");
    while (token != NULL) {
        token_count++;
        switch (token_count) {
            case 1: id_token = token; break;
            case 2: ta_token = token; break;
            case 3: ts_token = token; break;
            default: break;
        }
        token = strtok(NULL, "/");
    }
    if (token_count != 3) {
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    size_t id_len = strlen(id_token);
    if (id_len == 0 || id_len > 99) {
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    strcpy(new_man->id, id_token);
    char *endptr;
    long val = strtol(ta_token, &endptr, 10);
    if (*endptr != '\0' || val < INT_MIN || val > INT_MAX) {
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    new_man->ta = (int)val;
    val = strtol(ts_token, &endptr, 10);
    if (*endptr != '\0' || val < INT_MIN || val > INT_MAX) {
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    new_man->ts = (int)val;
    return error;
}


Error read_int(int* num){
    Error error = {0, 0};
    char line[256];

    if (fgets(line, sizeof(line), stdin) == NULL) {
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    line[strcspn(line, "\n")] = '\0';
    char* endptr;
    long value = strtol(line, &endptr, 10);
    if((*endptr != '\0') || (line[0] == '\0')){
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    if(value < 0 || value > INT_MAX){
        error.error_char = 1;
        error.number = 2;
        return error;
    }
    *num = (int)value;
    return error;
}