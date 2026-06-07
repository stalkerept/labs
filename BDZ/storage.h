#ifndef STORAGE_H
#define STORAGE_H

typedef struct Storage Storage;
Storage* storage_init(int capacity);
int storage_insert(Storage* s, const char* coords, int value);
int storage_find(Storage* s, const char* coords);
int storage_delete(Storage* s, const char* coords);
void storage_free(Storage* s);

#endif