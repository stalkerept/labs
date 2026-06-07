#ifndef FUNC_H
#define FUNC_H

#include "structures.h"

int is_empty_queue(Queue* queue);
Info pop_front(Queue* queue);
Error push_back(Queue* queue, Info value);
Error push_front(Queue* queue, Info value);
void clear_queue(Queue* queue);
void print_queue(Queue* queue);
char check_error(Error error, Queue* queue);
Error parse_patient(const char* token, Info* patient);
int cmp_patient(const void* a, const void* b);
void print_state(int time, Info* current, Queue* waiting);
Error read_patients(PatientData** patients, int* count);

#endif