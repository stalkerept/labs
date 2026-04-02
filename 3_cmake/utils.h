#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>

int read_unsigned_int(unsigned int *value);
int read_string(char *buffer, size_t max_len);
void clear_input();
void show_menu();
#endif