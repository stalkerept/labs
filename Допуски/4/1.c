#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"

/* Вспомогательная функция для рекурсивного удаления дерева */
void free_tree(Node* node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node->key);
    free(node->info);
    free(node);
}

/* Функция поиска узла по ключу (рекурсивная) */
Node* find_node(Node* root, const char* key) {
    if (!root) return NULL;
    int cmp = strcmp(key, root->key);
    if (cmp == 0) return root;
    if (cmp < 0) return find_node(root->left, key);
    return find_node(root->right, key);
}

/* Проверка свойства BST: для каждого узла key_left < key_node < key_right */
int is_bst(Node* node, char** min_key, char** max_key) {
    if (!node) return 1;
    if (min_key && strcmp(node->key, *min_key) <= 0) return 0;
    if (max_key && strcmp(node->key, *max_key) >= 0) return 0;
    return is_bst(node->left, min_key, &node->key) &&
           is_bst(node->right, &node->key, max_key);
}

/* Обёртка для проверки BST */
int check_bst(Node* root) {
    return is_bst(root, NULL, NULL);
}

/* Подсчёт количества узлов */
int count_nodes(Node* node) {
    if (!node) return 0;
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}

/* Проверка корректности родительских связей */
int check_parents(Node* node, Node* parent) {
    if (!node) return 1;
    if (node->parent != parent) return 0;
    return check_parents(node->left, node) &&
           check_parents(node->right, node);
}


int insert(Tree* tree, const char* key, const char* info){
    if(!tree || !key || !info) return 1;
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node -> key = strdup(key);
    new_node -> info = strdup(info);
    new_node -> left = NULL;
    new_node -> right = NULL;
    new_node -> parent = NULL;
    int hw = 0;
    if(!tree -> root){
        tree -> root = new_node;
        return 0;
    }
    else{
        Node* now = tree -> root;
        Node* prev = NULL;
        while(now){
            hw = strcmp(new_node -> key, now -> key);
            if(hw>0){
                prev = now;
                now = now -> right;
            }
            else if(hw < 0){
                prev = now;
                now = now -> left;
            }
            else{
                free(new_node -> key);
                free(new_node -> info);
                free(new_node);
                return 1;
            }
        }
        new_node -> parent = prev;
        if(hw>0) prev -> right = new_node;
        else prev -> left = new_node;
        return 0;
    }
};

int main() {
    /* Тест 1: Вставка в пустое дерево */
    {
        Tree tree = { .root = NULL };
        int res = insert(&tree, "key1", "value1");
        assert(res == 0);
        assert(tree.root != NULL);
        assert(strcmp(tree.root->key, "key1") == 0);
        assert(strcmp(tree.root->info, "value1") == 0);
        assert(tree.root->left == NULL);
        assert(tree.root->right == NULL);
        assert(tree.root->parent == NULL);
        free_tree(tree.root);
        printf("Test 1 passed\n");
    }

    /* Тест 2: Вставка нескольких элементов и проверка структуры */
    {
        Tree tree = { .root = NULL };
        assert(insert(&tree, "banana", "yellow") == 0);
        assert(insert(&tree, "apple", "green") == 0);
        assert(insert(&tree, "cherry", "red") == 0);
        assert(insert(&tree, "apricot", "orange") == 0);
        assert(insert(&tree, "blueberry", "blue") == 0);

        // Проверяем BST-свойство
        assert(check_bst(tree.root));

        // Проверяем родительские связи
        assert(check_parents(tree.root, NULL));

        // Проверяем количество узлов
        // printf("HEEEEY: %d", count_nodes(tree.root));
        assert(count_nodes(tree.root) == 5);

        // Проверяем наличие ключей и значений (поиск)
        Node* n = find_node(tree.root, "apricot");
        assert(n != NULL);
        assert(strcmp(n->info, "orange") == 0);

        n = find_node(tree.root, "cherry");
        assert(n != NULL);
        assert(strcmp(n->info, "red") == 0);

        free_tree(tree.root);
        printf("Test 2 passed\n");
    }

    /* Тест 3: Попытка вставки дубликата ключа */
    {
        Tree tree = { .root = NULL };
        assert(insert(&tree, "double", "first") == 0);
        assert(insert(&tree, "double", "second") == 1);   // ошибка
        assert(count_nodes(tree.root) == 1);
        Node* n = find_node(tree.root, "double");
        assert(n != NULL);
        assert(strcmp(n->info, "first") == 0);  // значение не перезаписалось
        free_tree(tree.root);
        printf("Test 3 passed\n");
    }

    /* Тест 4: Проверка копирования строк (независимость памяти) */
    {
        Tree tree = { .root = NULL };
        char key_buf[] = "key";
        char val_buf[] = "value";
        assert(insert(&tree, key_buf, val_buf) == 0);

        // Меняем исходные буферы
        key_buf[0] = 'X';
        val_buf[0] = 'Y';

        Node* n = tree.root;
        assert(strcmp(n->key, "key") == 0);    // ключ не изменился
        assert(strcmp(n->info, "value") == 0); // значение не изменилось
        free_tree(tree.root);
        printf("Test 4 passed\n");
    }

    /* Тест 5: Вставка в более сложную структуру, проверка left/right */
    {
        Tree tree = { .root = NULL };
        char* keys[] = {"m", "g", "t", "a", "h", "r", "z"};
        for (int i = 0; i < 7; i++) {
            assert(insert(&tree, keys[i], keys[i]) == 0);
        }
        // Проверяем структуру: m - корень, g - левый, t - правый
        assert(strcmp(tree.root->key, "m") == 0);
        assert(strcmp(tree.root->left->key, "g") == 0);
        assert(strcmp(tree.root->right->key, "t") == 0);
        assert(strcmp(tree.root->left->left->key, "a") == 0);
        assert(strcmp(tree.root->left->right->key, "h") == 0);
        assert(strcmp(tree.root->right->left->key, "r") == 0);
        assert(strcmp(tree.root->right->right->key, "z") == 0);
        assert(check_bst(tree.root));
        free_tree(tree.root);
        printf("Test 5 passed\n");
    }

    /* Тест 6: Вставка с очень длинными строками (проверка на переполнение буфера) */
    {
        Tree tree = { .root = NULL };
        char long_key[1024];
        char long_val[1024];
        memset(long_key, 'k', 1023);
        long_key[1023] = '\0';
        memset(long_val, 'v', 1023);
        long_val[1023] = '\0';
        assert(insert(&tree, long_key, long_val) == 0);
        Node* n = find_node(tree.root, long_key);
        assert(n != NULL);
        assert(strcmp(n->info, long_val) == 0);
        free_tree(tree.root);
        printf("Test 6 passed\n");
    }

    printf("All tests passed successfully!\n");
    return 0;
}