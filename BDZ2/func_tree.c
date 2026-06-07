#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "func_tree.h"


static void rotate_right(Leaf** root, Leaf* y) {
    Leaf* x = y->left;
    if (!x) return;
    Leaf* p = y->parent;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->right = y;
    y->parent = x;
    x->parent = p;
    if (p) {
        if (p->left == y) p->left = x;
        else p->right = x;
    } else {
        *root = x;
    }
}

static void rotate_left(Leaf** root, Leaf* y) {
    Leaf* x = y->right;
    if (!x) return;
    Leaf* p = y->parent;
    y->right = x->left;
    if (x->left) x->left->parent = y;
    x->left = y;
    y->parent = x;
    x->parent = p;
    if (p) {
        if (p->left == y) p->left = x;
        else p->right = x;
    } else {
        *root = x;
    }
}

Leaf* splay(Leaf* root, const char* coords) {
    if (!root) return NULL;
    Leaf* current = root;
    Leaf* last = NULL;
    while (current) {
        last = current;
        int cmp = strcmp(current->coords, coords);
        if (cmp == 0) break;
        if (cmp > 0)
            current = current->left;
        else
            current = current->right;
    }
    if (!last) return root;
    Leaf* new_root = root;
    while (last->parent) {
        Leaf* p = last->parent;
        Leaf* g = p->parent;
        if (!g) {
            if (p->left == last) rotate_right(&new_root, p);
            else rotate_left(&new_root, p);
        } else if (p->left == last && g->left == p) {
            rotate_right(&new_root, g);
            rotate_right(&new_root, p);
        } else if (p->right == last && g->right == p) {
            rotate_left(&new_root, g);
            rotate_left(&new_root, p);
        } else if (p->right == last && g->left == p) {
            rotate_left(&new_root, p);
            rotate_right(&new_root, g);
        } else {
            rotate_right(&new_root, p);
            rotate_left(&new_root, g);
        }
    }
    return new_root;
}

Leaf* find(Tree* tree, const char* coords) {
    if (!tree || !tree->root || !coords) return NULL;
    tree->root = splay(tree->root, coords);
    if (strcmp(tree->root->coords, coords) == 0)
        return tree->root;
    return NULL;
}

static Leaf* find_extreme(Leaf* node, char mode) {
    if (!node) return NULL;
    Leaf* prev = NULL;
    Leaf* cur = node;
    if (mode == 1)
        while (cur) { prev = cur; cur = cur->right; }
    else if (mode == 2)
        while (cur) { prev = cur; cur = cur->left; }
    return prev;
}

char insert(Tree* tree, int key, const char* coords) {
    if (!tree || key <= 0 || !coords) return 1;
    Leaf* new_leaf = (Leaf*)malloc(sizeof(Leaf));
    if (!new_leaf) return 1;
    new_leaf->key = key;
    new_leaf->coords = strdup(coords);
    if (!new_leaf->coords) {
        free(new_leaf);
        return 1;
    }
    new_leaf->left = new_leaf->right = new_leaf->parent = NULL;
    if (!tree->root) {
        tree->root = new_leaf;
        tree->count = 1;
        return 0;
    }
    Leaf* curr = tree->root;
    Leaf* parent = NULL;
    int cmp = 0;
    while (curr) {
        parent = curr;
        cmp = strcmp(coords, curr->coords);
        if (cmp > 0)
            curr = curr->right;
        else if (cmp < 0)
            curr = curr->left;
        else {
            curr->key = key;
            free(new_leaf->coords);
            free(new_leaf);
            tree->root = splay(tree->root, coords);
            return 2;
        }
    }
    new_leaf->parent = parent;
    if (cmp > 0)
        parent->right = new_leaf;
    else
        parent->left = new_leaf;
    tree->count++;
    tree->root = splay(tree->root, coords);
    return 0;
}

Tree* tree_init(void) {
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->count = 0;
    return tree;
}

Stack* stack_init(unsigned int capacity) {
    Stack* st = (Stack*)malloc(sizeof(Stack));
    if (!st) return NULL;
    st->top = -1;
    st->capacity = capacity;
    st->array = (Leaf**)malloc(sizeof(Leaf*) * capacity);
    if (!st->array) {
        free(st);
        return NULL;
    }
    return st;
}

static void stack_push(Stack* st, Leaf* node) {
    if (st->top + 1 >= st->capacity) {
        st->capacity *= 2;
        Leaf** tmp = realloc(st->array, sizeof(Leaf*) * st->capacity);
        if (!tmp) return;
        st->array = tmp;
    }
    st->array[++st->top] = node;
}

static Leaf* stack_pop(Stack* st) {
    if (st->top == -1) return NULL;
    return st->array[st->top--];
}

void free_tree(Tree* tree) {
    if (!tree) return;
    if (!tree->root) {
        free(tree);
        return;
    }
    Stack* st = stack_init(256);
    if (!st) {
        free(tree);
        return;
    }
    Leaf* curr = tree->root;
    Leaf* last_visited = NULL;
    while (st->top != -1 || curr) {
        if (curr) {
            stack_push(st, curr);
            curr = curr->left;
        } else {
            Leaf* peek = st->array[st->top];
            if (peek->right && last_visited != peek->right) {
                curr = peek->right;
            } else {
                last_visited = stack_pop(st);
                free(last_visited->coords);
                free(last_visited);
            }
        }
    }
    free(st->array);
    free(st);
    free(tree);
}

char delete(Tree* tree, const char* coords) {
    if (!tree || !tree->root || !coords) return 1;
    Leaf* victim = find(tree, coords);
    if (!victim) return 1;
    if (victim->left && victim->right) {
        Leaf* succ = find_extreme(victim->right, 2);
        free(victim->coords);
        victim->key = succ->key;
        victim->coords = succ->coords;
        succ->coords = NULL;
        victim = succ;
    }
    Leaf* child = victim->left ? victim->left : victim->right;
    Leaf* parent = victim->parent;
    if (parent) {
        if (parent->left == victim) parent->left = child;
        else parent->right = child;
    } else {
        tree->root = child;
    }
    if (child) child->parent = parent;
    free(victim->coords);
    free(victim);
    tree->count--;
    if (parent) {
        tree->root = splay(tree->root, parent->coords);
    }
    return 0;
}