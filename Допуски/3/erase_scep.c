#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Node {
    char* key;
    char* info;
    struct Node* next;
} Node;

typedef struct Table {
    Node** data;
    int capacity;
    unsigned long long (*hash)(char*);
} Table;



// ---------- Вспомогательные функции (те же, что и в тестах для insert) ----------
unsigned long long simple_hash(char* key) {
    unsigned long long h = 0;
    for (char* p = key; *p; p++)
        h += (unsigned char)(*p);
    return h;
}

Table* create_table(int capacity, unsigned long long (*hash)(char*)) {
    Table* t = (Table*)malloc(sizeof(Table));
    t->capacity = capacity;
    t->hash = hash;
    t->data = (Node**)calloc(capacity, sizeof(Node*));
    return t;
}

void free_table(Table* t) {
    if (!t) return;
    for (int i = 0; i < t->capacity; i++) {
        Node* cur = t->data[i];
        while (cur) {
            Node* next = cur->next;
            free(cur->key);
            free(cur->info);
            free(cur);
            cur = next;
        }
    }
    free(t->data);
    free(t);
}

int find_key(Table* t, char* key) {
    if (!t || !key) return 0;
    unsigned long long idx = t->hash(key) % t->capacity;
    Node* cur = t->data[idx];
    while (cur) {
        if (strcmp(cur->key, key) == 0) return 1;
        cur = cur->next;
    }
    return 0;
}

int count_elements(Table* t) {
    int cnt = 0;
    for (int i = 0; i < t->capacity; i++) {
        Node* cur = t->data[i];
        while (cur) {
            cnt++;
            cur = cur->next;
        }
    }
    return cnt;
}

// Корректная реализация insert для подготовки данных
int insert(Table* table, char* key, char* info) {
    if (!table || !table->data || !table->hash || !key || !info) return 1;
    unsigned long long idx = table->hash(key) % table->capacity;

    // Проверка дубликата
    Node* cur = table->data[idx];
    while (cur) {
        if (strcmp(cur->key, key) == 0) return 1;
        cur = cur->next;
    }

    char* key_copy = strdup(key);
    if (!key_copy) return 1;
    char* info_copy = strdup(info);
    if (!info_copy) {
        free(key_copy);
        return 1;
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        free(key_copy);
        free(info_copy);
        return 1;
    }

    new_node->key = key_copy;
    new_node->info = info_copy;
    new_node->next = table->data[idx];
    table->data[idx] = new_node;
    return 0;
}

int erase(Table* table, char* key){
    if(!table || !(table -> data) || !(table->hash) || !key) return 1;
    unsigned long long k = (table -> hash(key))%(table -> capacity);
    Node* n = table -> data[k];
    Node* p = NULL;
    while(n && strcmp(n -> key, key) != 0){
        p = n;
        n = n->next;
    }
    if(!n) return 1;
    if(!p){
        table -> data[k] = n->next;
    }
    else{
        p -> next = n -> next;
    }
    free(n -> info);
    free(n);
    return 0;
}

// ---------- Тесты для erase ----------
int main() {
    // Тест 1: удаление существующего элемента из цепочки (середина)
    Table* t1 = create_table(5, simple_hash);
    insert(t1, "one", "один");
    insert(t1, "two", "два");
    insert(t1, "three", "три");
    int res = erase(t1, "two");
    assert(res == 0);
    assert(count_elements(t1) == 2);
    assert(find_key(t1, "one") && find_key(t1, "three"));
    assert(!find_key(t1, "two"));
    printf("Тест 1 пройден\n");
    free_table(t1);

    // Тест 2: удаление несуществующего ключа
    Table* t2 = create_table(5, simple_hash);
    insert(t2, "key", "value");
    res = erase(t2, "absent");
    assert(res == 1);
    assert(count_elements(t2) == 1);
    assert(find_key(t2, "key"));
    printf("Тест 2 пройден\n");
    free_table(t2);

    // Тест 3: удаление из пустой таблицы
    Table* t3 = create_table(5, simple_hash);
    res = erase(t3, "any");
    assert(res == 1);
    assert(count_elements(t3) == 0);
    printf("Тест 3 пройден\n");
    free_table(t3);

    // Тест 4: удаление единственного элемента
    Table* t4 = create_table(5, simple_hash);
    insert(t4, "sole", "only");
    res = erase(t4, "sole");
    assert(res == 0);
    assert(count_elements(t4) == 0);
    assert(t4->data[simple_hash("sole") % 5] == NULL);
    printf("Тест 4 пройден\n");
    free_table(t4);

    // Тест 5: удаление первого элемента в цепочке (головы)
    Table* t5 = create_table(3, simple_hash);
    insert(t5, "ab", "first");
    insert(t5, "ba", "second"); // коллизия с "ab"
    insert(t5, "ac", "third");  // другой хеш
    res = erase(t5, "ab");
    assert(res == 0);
    assert(count_elements(t5) == 2);
    assert(find_key(t5, "ba") && find_key(t5, "ac"));
    assert(!find_key(t5, "ab"));
    // Проверяем, что голова цепочки теперь "ba"
    unsigned long long idx = simple_hash("ab") % 3;
    Node* head = t5->data[idx];
    assert(head && strcmp(head->key, "ba") == 0);
    printf("Тест 5 пройден\n");
    free_table(t5);

    // Тест 6: удаление последнего элемента в цепочке
    Table* t6 = create_table(3, simple_hash);
    insert(t6, "ab", "first");
    insert(t6, "ba", "second");
    res = erase(t6, "ba");
    assert(res == 0);
    assert(count_elements(t6) == 1);
    assert(find_key(t6, "ab"));
    assert(!find_key(t6, "ba"));
    // Проверяем, что у головы next == NULL
    Node* cur6 = t6->data[simple_hash("ab") % 3];
    assert(cur6 && cur6->next == NULL);
    printf("Тест 6 пройден\n");
    free_table(t6);

    // Тест 7: удаление из таблицы с несколькими цепочками (проверка, что другие не затронуты)
    Table* t7 = create_table(5, simple_hash);
    insert(t7, "cat", "кошка");
    insert(t7, "dog", "собака");
    insert(t7, "mouse", "мышь");
    res = erase(t7, "dog");
    assert(res == 0);
    assert(count_elements(t7) == 2);
    assert(find_key(t7, "cat") && find_key(t7, "mouse"));
    printf("Тест 7 пройден\n");
    free_table(t7);

    // Тест 8: передача NULL таблицы
    res = erase(NULL, "key");
    assert(res == 1);
    printf("Тест 8 пройден\n");

    // Тест 9: передача NULL ключа
    Table* t9 = create_table(5, simple_hash);
    insert(t9, "key", "value");
    res = erase(t9, NULL);
    assert(res == 1);
    assert(count_elements(t9) == 1);
    free_table(t9);
    printf("Тест 9 пройден\n");

    // Тест 10: повторное удаление после успешного удаления
    Table* t10 = create_table(5, simple_hash);
    insert(t10, "x", "X");
    res = erase(t10, "x");
    assert(res == 0);
    res = erase(t10, "x");
    assert(res == 1);
    assert(count_elements(t10) == 0);
    free_table(t10);
    printf("Тест 10 пройден\n");

    printf("Все тесты для функции erase пройдены успешно!\n");
    return 0;
}