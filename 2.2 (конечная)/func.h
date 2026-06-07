#ifndef FUNC_H
#define FUNC_H

#include "structures.h"

// Унифицированные функции для работы с очередью
void init_queue(Queue* q, int impl);
void clear_queue(Queue* q);
int is_empty_queue(Queue* q);
Error push_queue(Queue* q, Info value);
Info pop_front_queue(Queue* q);
void print_queue(Queue* q);

// Общие функции
char check_error(Error error, Queue* queue);
Error parse_patient(const char* token, Info* patient);
int cmp_patient(const void* a, const void* b);
void print_state(int time, Info* current, Queue* waiting);
Error read_patients(PatientData** patients, int* count);

#endif