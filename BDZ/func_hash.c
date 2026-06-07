#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include <stdint.h>
#include <string.h>

 // это sdbm, можете ченкуть в инете про неё
unsigned long hash_string(unsigned char *str){
    unsigned long hash = 0;
    int c;
    while (c = *str++) hash = c + (hash << 6) + (hash << 16) - hash;
    return hash;
}

int find(Table* table, char* coords) {
    if (!table || !coords || !table->data || table->len <= 0) return -1;
    int start = hash_string((unsigned char*)coords) % table->capasity;
    int i = start;
    while (1) {
        if (table->data[i].data != 0) {
            if (strcmp(table->data[i].coords, coords) == 0)
                return i;
        }
        i = (i + 1) % table->capasity;
        if (i == start) break;
    }
    return -1;
}

char insert(Table* table, int key, char* coords, unsigned long (*hash)(unsigned char*)) {
    if (!table || !coords || !table->data || table->len == table->capasity) return 1;
    int exist_idx = find(table, coords);
    if (exist_idx != -1) {
        table->data[exist_idx].data = key;
        return 2;
    }
    char* new_coords = (char*)malloc(strlen(coords) + 1);
    if (!new_coords) return 1;
    strcpy(new_coords, coords);
    Node new_node = {key, new_coords};
    int start = hash((unsigned char*)coords) % table->capasity;
    int i = start;
    while (table->data[i].data != 0) {
        i = (i + 1) % table->capasity;
        if (i == start) return 1;
    }
    table->data[i] = new_node;
    table->len++;
    return 0;
}

char delete(Table* table, const char* coords, unsigned long (*hash)(unsigned char*)) {
    if (!table || !table->data || table->len <= 0 || !coords) return 1;
    int idx = find(table, (char*)coords);
    if (idx == -1) return 1;
    free(table->data[idx].coords);
    table->data[idx].data = 0;
    table->data[idx].coords = NULL;
    table->len--;
    return 0;
}

Table* init_table(int capasity) {
    Table* table = (Table*)malloc(sizeof(Table));
    if (!table) return NULL;
    table->capasity = capasity, table->len = 0;
    if (capasity > 0) {
        table->data = calloc(capasity, sizeof(Node));
        if (!table->data) {
            free(table);
            return NULL;
        }
    } else {
        table->data = NULL;
    }
    return table;
}

void free_table(Table* table) {
    if (!table || !table->data) return;
    for (int i = 0; i < table->capasity; i++) {
        free(table->data[i].coords);
    }
    free(table->data);
    free(table);
}