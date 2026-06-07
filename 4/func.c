#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"

Tree* tree_init(void){
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    tree -> root = NULL;
    tree -> count = 0;
    return tree;
}

Stack* stack_init(unsigned int capasity){
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(!stack) return NULL;
    stack -> top = -1;
    stack -> capasity = capasity;
    stack -> array = (Node**)malloc(sizeof(Node*) * stack->capasity);
    return stack;
}

void free_tree(Tree* tree){
    if(!tree || !tree -> root || tree -> count <= 0) return;
    Stack* stack = stack_init(tree -> count);
    Node* now = tree -> root;
    Node* lastvisit =  NULL;
    while(stack -> top != -1 || now){
        if(now){
            stack -> array[++stack->top] = now;
            now = now -> left;
        }
        else{
            Node* peek = stack -> array[stack -> top];
            if(peek -> right && lastvisit != peek->right) now = peek -> right;
            else{
                lastvisit = stack -> array[stack->top --];
                free(lastvisit -> key);
                free(lastvisit -> info);
                free(lastvisit);
            }
        }
    }
    free(stack -> array);
    free(stack);
}

char* insert(Tree* tree, const char* key, const char* info) {
    if (!tree || !key || !info) return NULL;
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->key = strdup(key);
    new_node->info = strdup(info);
    if (!new_node->key || !new_node->info) {
        free(new_node->key);
        free(new_node->info);
        free(new_node);
        return NULL;
    }
    new_node->right = new_node->left = new_node->parent = NULL;
    if (!tree->root) {
        tree->root = new_node;
        tree->count++;
        return NULL;
    }
    Node *prev = NULL, *curr = tree->root;
    int cmp = 0;
    while (curr) {
        prev = curr;
        cmp = strcmp(new_node->key, curr->key);
        if (cmp > 0)
            curr = curr->right;
        else if (cmp < 0)
            curr = curr->left;
        else {
            char* old_info = strdup(curr->info);
            if (!old_info) {
                free(new_node->info);
                free(new_node->key);
                free(new_node);
                return NULL;
            }
            free(curr->info);
            curr->info = new_node->info;
            free(new_node->key);
            free(new_node);
            return old_info;
        }
    }
    new_node->parent = prev;
    if (cmp > 0)
        prev->right = new_node;
    else
        prev->left = new_node;
    tree->count++;
    return NULL;
}

Node* find(Tree* tree, const char* key){
    if(!tree || !tree->root || !key) return NULL;
    Node* now = tree -> root;
    int hw = 0;
    while(now){
        hw = strcmp(key, now->key);
        if(hw > 0) now = now -> right;
        else if(hw < 0) now = now -> left;
        else return now;
    }
    return NULL;
}

// mode: 1 - max, 2 - min.
Node* find_ex(Node* node, char mode){
    if(!node) return NULL;
    Node* prev = NULL;
    Node* now = node;
    if(mode == 1) while(now){prev = now; now = now -> right;}
    else if (mode == 2) while(now){prev = now; now = now -> left;}
    return prev;
}

Node* find_special(Tree* tree, const char* key) {
    if (!tree || !tree->root || !key) return NULL;
    Node* curr = tree->root;
    Node* candidate = NULL;
    while (curr) {
        int cmp = strcmp(key, curr->key);
        if (cmp < 0) {
            candidate = curr;
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return candidate;
}

char delete(Tree* tree, const char* key) {
    Node* victim = find(tree, key);
    if (!victim) return 1;
    if (victim->left && victim->right) {
        Node* successor = find_ex(victim->right, 2);
        char* new_key = strdup(successor->key);
        char* new_info = strdup(successor->info);
        if (!new_key || !new_info) {
            free(new_key); free(new_info);
            return 1;
        }
        free(victim->key);
        free(victim->info);
        victim->key = new_key;
        victim->info = new_info;
        victim = successor;
    }
    Node* child = victim->left ? victim->left : victim->right;
    Node* parent = victim->parent;
    if (parent) {
        if (parent->left == victim) parent->left = child;
        else parent->right = child;
    } 
    else{
        tree->root = child;
    }
    if (child) child->parent = parent;
    free(victim->key);
    free(victim->info);
    free(victim);
    tree->count--;
    return 0;
}
