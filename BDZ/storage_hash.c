#include "storage.h"
#include "structures.h"
#include <stdlib.h>
#include "func_hash.h"

struct Storage {
    Table* table;
};


Storage* storage_init(int capacity) {
    Storage* s = malloc(sizeof(*s));
    if (!s) return NULL;
    s->table = init_table(capacity);
    if (!s->table) {
        free(s);
        return NULL;
    }
    return s;
}

int storage_insert(Storage* s, const char* coords, int value) {
    if (!s) return 1;
    return insert(s->table, value, (char*)coords, hash_string);
}

int storage_find(Storage* s, const char* coords) {
    if (!s) return -1;
    int idx = find(s->table, (char*)coords);
    if (idx == -1) return -1;
    return s->table->data[idx].data;
}

int storage_delete(Storage* s, const char* coords) {
    if (!s) return 1;
    return delete(s->table, coords, hash_string);
}

void storage_free(Storage* s) {
    if (!s) return;
    free_table(s->table);
    free(s);
}