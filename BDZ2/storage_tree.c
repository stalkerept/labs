#include "storage.h"
#include "structures.h"
#include <stdlib.h>
#include "func_tree.h"

struct Storage {
    Tree* tree;
    int max_tile;
};

Storage* storage_init(int capacity) {
    (void)capacity;
    Storage* s = malloc(sizeof(*s));
    if (!s) return NULL;
    s->tree = tree_init();
    if (!s->tree) {
        free(s);
        return NULL;
    }
    s->max_tile = 0;
    return s;
}

int storage_insert(Storage* s, const char* coords, int value) {
    if (!s) return 1;
    int res = insert(s->tree, value, coords);
    if (res == 0 || res == 2) {
        if (value > s->max_tile)
            s->max_tile = value;
    }
    return res;
}

int storage_find(Storage* s, const char* coords) {
    if (!s) return -1;
    Leaf* node = find(s->tree, (char*)coords);
    return node ? node->key : -1;
}

int storage_delete(Storage* s, const char* coords) {
    if (!s) return 1;
    return delete(s->tree, coords);
}

void storage_free(Storage* s) {
    if (!s) return;
    free_tree(s->tree);
    free(s);
}

int storage_have_empty_cell(Storage* s, int size) {
    if (!s) return -1;
    return (s->tree->count < size * size) ? 1 : 0;
}

int storage_max_tile(Storage* s) {
    if (!s) return -1;
    return s->max_tile;
}