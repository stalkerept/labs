#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Node {
    int key;
    char* info;
} Node;

typedef struct Table {
    Node* data;
    int capacity;
    int size;
} Table;

// Прототип тестируемой функции
int find(Table* table, int key){
    if(!table || !(table->data)) return -1;
    for(int i = 0; i<table->size; i++){
        if(table -> data[i].key == key){
            return i;
        }
    }
    return -1;
}

int insert(Table* table, int key, char* info){
    if(!table || !(table -> data)|| table -> size >= table -> capacity || !info){
        return 1;
    }
    int index = find(table, key);
    if(index >= 0) return 1;
    table -> data[table -> size].info = info;
    table -> data[table -> size].key = key;
    table -> size++;
    return 0; 
}
// Вспомогательные функции для работы с таблицей
void init_table(Table* table, int capacity) {
    table->data = (Node*)malloc(capacity * sizeof(Node));
    table->capacity = capacity;
    table->size = 0;
}

void free_table(Table* table) {
    free(table->data);
    table->data = NULL;
    table->capacity = 0;
    table->size = 0;
}

int main() {
    Table table;

    // Тест 1: вставка в пустую таблицу
    init_table(&table, 3);
    int result = insert(&table, 10, "hello");
    assert(result == 0);
    assert(table.size == 1);
    assert(table.data[0].key == 10);
    assert(strcmp(table.data[0].info, "hello") == 0);
    printf("Тест 1 пройден\n");

    // Тест 2: вставка второго элемента с другим ключом
    result = insert(&table, 20, "world");
    assert(result == 0);
    assert(table.size == 2);
    assert(table.data[1].key == 20);
    assert(strcmp(table.data[1].info, "world") == 0);
    printf("Тест 2 пройден\n");

    // Тест 3: попытка вставить дубликат ключа
    result = insert(&table, 10, "duplicate");
    assert(result == 1);
    assert(table.size == 2);
    assert(strcmp(table.data[0].info, "hello") == 0);
    printf("Тест 3 пройден\n");

    // Тест 4: заполнение таблицы до предела
    result = insert(&table, 30, "foo");
    assert(result == 0);
    assert(table.size == 3);
    assert(table.data[2].key == 30);
    assert(strcmp(table.data[2].info, "foo") == 0);
    printf("Тест 4 пройден\n");

    // Тест 5: вставка в уже заполненную таблицу
    result = insert(&table, 40, "bar");
    assert(result == 1);
    assert(table.size == 3);
    printf("Тест 5 пройден\n");

    // Тест 6: проверка, что capacity не изменилась
    assert(table.capacity == 3);
    printf("Тест 6 пройден\n");

    // Тест 7: вставка с отрицательным ключом
    Table table2;
    init_table(&table2, 2);
    result = insert(&table2, -5, "negative");
    assert(result == 0);
    assert(table2.size == 1);
    assert(table2.data[0].key == -5);
    assert(strcmp(table2.data[0].info, "negative") == 0);
    printf("Тест 7 пройден\n");
    free_table(&table2);

    // Тест 8: вставка в таблицу с capacity = 0
    Table table3;
    init_table(&table3, 0);
    result = insert(&table3, 1, "zero_cap");
    assert(result == 1);
    assert(table3.size == 0);
    printf("Тест 8 пройден\n");
    free_table(&table3);

    // Тест 9: корректная работа после неудачной вставки дубликата
    Table table4;
    init_table(&table4, 2);
    result = insert(&table4, 100, "first");
    assert(result == 0);
    result = insert(&table4, 100, "second");
    assert(result == 1);
    result = insert(&table4, 200, "third");
    assert(result == 0);
    assert(table4.size == 2);
    assert(table4.data[0].key == 100 && strcmp(table4.data[0].info, "first") == 0);
    assert(table4.data[1].key == 200 && strcmp(table4.data[1].info, "third") == 0);
    printf("Тест 9 пройден\n");
    free_table(&table4);

    // Тест 10: вставка нескольких элементов с одинаковой информацией
    Table table5;
    init_table(&table5, 3);
    result = insert(&table5, 1, "same");
    assert(result == 0);
    result = insert(&table5, 2, "same");
    assert(result == 0);
    result = insert(&table5, 3, "same");
    assert(result == 0);
    assert(table5.size == 3);
    for (int i = 0; i < 3; i++) {
        assert(table5.data[i].key == i + 1);
        assert(strcmp(table5.data[i].info, "same") == 0);
    }
    printf("Тест 10 пройден\n");
    free_table(&table5);

    // Очистка основной таблицы
    free_table(&table);

    printf("Все тесты пройдены успешно!\n");
    return 0;
}
