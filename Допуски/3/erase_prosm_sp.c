#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Node {
    int key;
    char* info;
    struct Node* next;
} Node;

typedef struct Table {
    Node* head;
} Table;


int erase(Table* table, int key){
    if(!table || !(table->head)) return 1;
    Node* n = table -> head;
    Node* p = NULL;
    while(n && n->key != key){
        p = n;
        n = n->next;
    }
    if(!n) return 1;
    if(p==NULL){
        table -> head = n -> next;
    }
    else{
        p -> next = n -> next;
    }
    free(n -> info);
    free(n);

    return 0;
}

// Вспомогательные функции для тестов (реализуются самостоятельно, но здесь приведены для полноты)
void init_table(Table* table) {
    table->head = NULL;
}

void free_table(Table* table) {
    Node* current = table->head;
    while (current) {
        Node* next = current->next;
        free(current->info);
        free(current);
        current = next;
    }
    table->head = NULL;
}

int find_key(const Table* table, int key) {
    Node* current = table->head;
    while (current) {
        if (current->key == key) return 1;
        current = current->next;
    }
    return 0;
}

int count_elements(const Table* table) {
    int cnt = 0;
    Node* current = table->head;
    while (current) {
        cnt++;
        current = current->next;
    }
    return cnt;
}

// Вспомогательная функция для получения узла по ключу (для проверки содержимого)
const char* get_info(const Table* table, int key) {
    Node* current = table->head;
    while (current) {
        if (current->key == key) return current->info;
        current = current->next;
    }
    return NULL;
}

// Реализация insert для подготовки тестов (должна быть корректной)
int insert(Table* table, int key, char* info) {
    if (!table || !info) return 1;
    if (find_key(table, key)) return 1;
    char* copy = strdup(info);
    if (!copy) return 1;
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        free(copy);
        return 1;
    }
    new_node->key = key;
    new_node->info = copy;
    new_node->next = NULL;
    if (!table->head) {
        table->head = new_node;
    } else {
        Node* cur = table->head;
        while (cur->next) cur = cur->next;
        cur->next = new_node;
    }
    return 0;
}

int main() {
    // Тест 1: удаление существующего элемента из середины списка
    Table t1;
    init_table(&t1);
    insert(&t1, 10, "ten");
    insert(&t1, 20, "twenty");
    insert(&t1, 30, "thirty");
    int result = erase(&t1, 20);
    assert(result == 0);
    assert(count_elements(&t1) == 2);
    assert(find_key(&t1, 20) == 0);
    assert(find_key(&t1, 10) == 1);
    assert(find_key(&t1, 30) == 1);
    // Проверяем порядок: 10 -> 30
    Node* cur = t1.head;
    assert(cur->key == 10);
    assert(strcmp(cur->info, "ten") == 0);
    cur = cur->next;
    assert(cur->key == 30);
    assert(strcmp(cur->info, "thirty") == 0);
    assert(cur->next == NULL);
    free_table(&t1);
    printf("Тест 1 пройден\n");

    // Тест 2: удаление несуществующего ключа
    Table t2;
    init_table(&t2);
    insert(&t2, 5, "five");
    result = erase(&t2, 42);
    assert(result == 1);
    assert(count_elements(&t2) == 1);
    assert(find_key(&t2, 5) == 1);
    free_table(&t2);
    printf("Тест 2 пройден\n");

    // Тест 3: удаление из пустого списка
    Table t3;
    init_table(&t3);
    result = erase(&t3, 100);
    assert(result == 1);
    assert(count_elements(&t3) == 0);
    free_table(&t3);
    printf("Тест 3 пройден\n");

    // Тест 4: удаление единственного элемента
    Table t4;
    init_table(&t4);
    insert(&t4, 42, "answer");
    result = erase(&t4, 42);
    assert(result == 0);
    assert(count_elements(&t4) == 0);
    assert(t4.head == NULL);
    free_table(&t4);
    printf("Тест 4 пройден\n");

    // Тест 5: удаление первого элемента (головы)
    Table t5;
    init_table(&t5);
    insert(&t5, 1, "first");
    insert(&t5, 2, "second");
    insert(&t5, 3, "third");
    result = erase(&t5, 1);
    assert(result == 0);
    assert(count_elements(&t5) == 2);
    assert(find_key(&t5, 1) == 0);
    assert(find_key(&t5, 2) == 1);
    assert(find_key(&t5, 3) == 1);
    // Проверяем порядок: 2 -> 3
    cur = t5.head;
    assert(cur->key == 2);
    assert(strcmp(cur->info, "second") == 0);
    cur = cur->next;
    assert(cur->key == 3);
    assert(strcmp(cur->info, "third") == 0);
    free_table(&t5);
    printf("Тест 5 пройден\n");

    // Тест 6: удаление последнего элемента
    Table t6;
    init_table(&t6);
    insert(&t6, 5, "five");
    insert(&t6, 6, "six");
    insert(&t6, 7, "seven");
    result = erase(&t6, 7);
    assert(result == 0);
    assert(count_elements(&t6) == 2);
    assert(find_key(&t6, 5) == 1);
    assert(find_key(&t6, 6) == 1);
    assert(find_key(&t6, 7) == 0);
    // Проверяем, что последний элемент стал 6
    cur = t6.head;
    while (cur->next) cur = cur->next;
    assert(cur->key == 6);
    free_table(&t6);
    printf("Тест 6 пройден\n");

    // Тест 7: передача NULL таблицы
    result = erase(NULL, 10);
    assert(result == 1);
    printf("Тест 7 пройден\n");

    // Тест 8: повторное удаление после успешного удаления
    Table t8;
    init_table(&t8);
    insert(&t8, 99, "ninety nine");
    result = erase(&t8, 99);
    assert(result == 0);
    result = erase(&t8, 99);
    assert(result == 1);
    assert(count_elements(&t8) == 0);
    free_table(&t8);
    printf("Тест 8 пройден\n");

    // Тест 9: удаление элемента, который является единственным в списке (уже проверено, но добавим ещё)
    Table t9;
    init_table(&t9);
    insert(&t9, 777, "lucky");
    result = erase(&t9, 777);
    assert(result == 0);
    assert(t9.head == NULL);
    free_table(&t9);
    printf("Тест 9 пройден\n");

    // Тест 10: последовательные вставки и удаления, проверка корректности памяти
    Table t10;
    init_table(&t10);
    insert(&t10, 1, "one");
    insert(&t10, 2, "two");
    insert(&t10, 3, "three");
    erase(&t10, 2);
    insert(&t10, 4, "four");
    erase(&t10, 1);
    assert(count_elements(&t10) == 2);
    assert(find_key(&t10, 3) && find_key(&t10, 4));
    // Проверяем порядок: 3 -> 4
    cur = t10.head;
    assert(cur->key == 3);
    assert(strcmp(cur->info, "three") == 0);
    cur = cur->next;
    assert(cur->key == 4);
    assert(strcmp(cur->info, "four") == 0);
    free_table(&t10);
    printf("Тест 10 пройден\n");

    printf("Все тесты для функции erase пройдены успешно!\n");
    return 0;
}