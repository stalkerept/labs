#ifndef FUNC_H
#define FUNC_H
#include "structures.h"

char is_clear(Queue* queue);
Error push(Queue* queue, Info value);
void print_queue(Queue* queue);
void clear_queue(Queue* queue);
char check_error(Error error, Queue* queue);
Error read_info(Info* new_man);
Error read_int(int* num);
#endif