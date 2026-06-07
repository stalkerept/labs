#include "storage.h"
#include "structures.h"
#include <stdlib.h>
#include "func_hash.h"
#include "hash.h"

struct Storage {
    Table* table;
    int max_tile;
};

Storage* storage_init(int capacity) {
    Storage* s = malloc(sizeof(*s));
    if (!s) return NULL;
    s->table = init_table(capacity, hash_string);
    if (!s->table) {
        free(s);
        return NULL;
    }
    s->max_tile = 0;
    return s;
}

int storage_insert(Storage* s, const char* coords, int value) {
    if (!s) return 1;
    int res = insert(s->table, value, (char*)coords);
    if (res == 0 || res == 2) {
        if (value > s->max_tile)
            s->max_tile = value;
    }
    return res;
}

int storage_find(Storage* s, const char* coords) {
    if (!s) return -1;
    int idx = find(s->table, (char*)coords);
    if (idx == -1) return -1;
    return s->table->data[idx].data;
}

int storage_delete(Storage* s, const char* coords) {
    if (!s) return 1;
    return delete(s->table, coords);
}

void storage_free(Storage* s) {
    if (!s) return;
    free_table(s->table);
    free(s);
}

int storage_have_empty_cell(Storage* s, int size) {
    if (!s) return -1;
    return (s->table->len < size * size) ? 1 : 0;
}

int storage_max_tile(Storage* s) {
    if (!s) return -1;
    return s->max_tile;
}