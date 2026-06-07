#include "storage.h"
#include "structures.h"
#include <stdlib.h>
#include "func_tree.h"

struct Storage {
    Tree* tree;
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
    return s;
}

int storage_insert(Storage* s, const char* coords, int value) {
    if (!s) return 1;
    return insert(s->tree, value, coords);
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