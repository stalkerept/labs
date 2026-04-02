#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include <string.h>

int init(Table* table, int msize){
    table -> ks = (KeySpace*)malloc(sizeof(KeySpace)*msize);
    if(!table -> ks) return 1;
    table -> msize = msize;
    table -> size = 0;
    for(int i = 0; i<msize; i++){
        table -> ks[i].node = NULL;
    }
    return 0;
}


int find(Table* table, unsigned int key, int release){
    if(!table || !table->ks || table -> size == 0) return -1;
    for(int i = 0; i<table->msize; i++){
        if(table -> ks[i].node != NULL && table -> ks[i].key == key){
            if(release == 0) return i;
            else{
                struct Node* n = table -> ks[i].node;
                while(n){
                    if(n->release == release){
                        return i;
                    }
                    n = n->next;
                }
                return -1;
            }
        }
    }
    return -1;
}

void free_table(Table* table){
    if(!table || !table -> ks) return;
    for(int i = 0; i< table ->msize; i++){
        struct Node* n = table -> ks[i].node;
        while(n){
            struct Node* tmp = n;
            n = n->next;
            free(tmp->info);
            free(tmp);
        }
    }
    free(table -> ks);
}

int insert(Table* table, unsigned int key, const char* info){
    if(!table || !info) return 1;
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    if(!new_node){
        return 1;
    }
    char* new_info = strdup(info);
    if(!new_info){
        free(new_node);
        return 1;
    }
    int k = find(table, key, 0);
    new_node -> release = 1;
    new_node -> info = new_info;
    new_node -> next = NULL;
    if(k<0){
        if(table -> size == table -> msize){
            free(new_node);
            free(new_info);
            return 1;
        }
        table -> ks[table->size].node = new_node;
        table -> ks[table->size].key = key;
        table -> size++;
        return 0;
    }
    struct Node* n = table->ks[k].node;
    new_node -> release = n -> release + 1;
    new_node -> next = table -> ks[k].node;
    table -> ks[k].node = new_node;
    return 0;
}


int erase(Table* table, unsigned int key, int release, char mode){
    if(!table || !table->ks || table->size==0) return 1;
    int k = find(table, key, 0);
    if(k < 0) return 1;
    if(release > 0){
        struct Node* n = table->ks[k].node;
        struct Node* p = NULL;
        while(n){
            if(n->release == release){
                if(!p) table -> ks[k].node = n->next;
                else p -> next = n -> next;
                free(n -> info);
                free(n);
                n = NULL;
                if(!table ->ks[k].node){
                    if(k != table -> size - 1){
                        table->ks[k] = table->ks[table -> size - 1];
                        table->ks[table -> size - 1].node = NULL;
                    }
                    table -> size--;
                }
                return 0;
            }
            p = n;
            n = n -> next;
        }
        return 1;
    }
    if(mode == 1){
        struct Node* n = table->ks[k].node;
        while(n){
            struct Node* p = n;
            n = n->next;
            free(p -> info);
            free(p);
        }
        if (k != table->size - 1) {
            table->ks[k] = table->ks[table->size - 1];
            table->ks[table->size - 1].node = NULL;
        } else {
            table->ks[k].node = NULL;
        }
        table->size--;
        return 0;
    }
    else{
        struct Node* n = table->ks[k].node;
        table -> ks[k].node = n -> next;
        free(n -> info);
        free(n);
        if(!table -> ks[k].node){
            if(k != table -> size - 1){
                table->ks[k] = table->ks[table -> size - 1];
                table->ks[table -> size - 1].node = NULL;
            }
            table -> size--;
        }
        return 0;
    }
}

void print_table(Table* table){
    for(int i = 0; i<table->msize; i++){
        struct Node* n = table -> ks[i].node;
        if(!n){
            printf("NULL\n");
        }
        else{
            printf("Key = %d: ", table -> ks[i].key);
            while(n -> next){
                printf("%d - %s -> ", n->release, n->info);
                n = n -> next;
            }
            printf("%d - %s", n->release, n->info);
            printf("\n");
        }
    }
    return;
}

Table* find_individual(Table* table, unsigned int key, int release){
    Table* new_table = (Table*)malloc(sizeof(Table));
    if(init(new_table, 1)){
        free(new_table);
        return NULL;
    }
    int k = find(table, key, release);
    if(k>=0){
        struct Node* n = table -> ks[k].node;
        if(release == 0){
            while(n){
                if(insert(new_table, key, n->info) == 1){
                    free_table(new_table);
                    free(new_table);
                    return NULL;
                }
                n = n->next;
            }
        }
        else{
            while(n){
                if(n -> release == release){
                    if(insert(new_table, key, n->info) == 1){
                        free_table(new_table);
                        free(new_table);
                        return NULL;
                    }
                    break;
                }
                n = n->next;
            }
        }
        
    }
    return new_table;
}
