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



// Простая реализация find для тестов
int find(const Table* table, int key) {
    if (!table || !table->data) return -1;
    for (int i = 0; i < table->size; i++) {
        if (table->data[i].key == key) return i;
    }
    return -1;
}

int erase(Table* table, int key){
    if(!table || !(table -> data) || table -> size <= 0) return 1;
    int index = find(table, key);
    if(index < 0) return 1;
    free(table -> data[index].info);
    for(int i = index; i < table->size-1; i++){
        table -> data[i] = table -> data[i+1];
    }
    table -> data[table -> size - 1].info = NULL;
    table -> data[table -> size - 1].key = 0;
    table -> size --;
    return 0;
}

// Вспомогательные функции для тестов
void init_table(Table* table, int capacity) {
    table->data = (Node*)malloc(capacity * sizeof(Node));
    table->capacity = capacity;
    table->size = 0;
}

void free_table(Table* table) {
    if (table->data) {
        // Освобождаем строки, если они были скопированы
        for (int i = 0; i < table->size; i++) {
            free(table->data[i].info);
        }
        free(table->data);
    }
    table->data = NULL;
    table->capacity = 0;
    table->size = 0;
}

int insert(Table* table, int key, char* info) {
    if (!table || !table->data || table->size >= table->capacity || !info)
        return 1;
    if (find(table, key) >= 0)
        return 1;

    char* copy = strdup(info);
    if (!copy) return 1;

    table->data[table->size].key = key;
    table->data[table->size].info = copy;
    table->size++;
    return 0;
}

// Основная программа с тестами
int main() {
    Table table;

    // Тест 1: удаление существующего элемента из непустой таблицы
    init_table(&table, 5);
    insert(&table, 10, "ten");
    insert(&table, 20, "twenty");
    insert(&table, 30, "thirty");
    int result = erase(&table, 20);
    assert(result == 0);
    assert(table.size == 2);
    assert(find(&table, 20) == -1);
    assert(find(&table, 10) != -1);
    assert(find(&table, 30) != -1);
    // Проверяем, что порядок сохранён (20 был вторым, теперь на его месте 30)
    assert(table.data[0].key == 10);
    assert(table.data[1].key == 30);
    printf("Тест 1 пройден\n");

    // Тест 2: удаление несуществующего ключа
    result = erase(&table, 999);
    assert(result == 1);
    assert(table.size == 2);
    assert(find(&table, 10) != -1);
    assert(find(&table, 30) != -1);
    printf("Тест 2 пройден\n");

    // Тест 3: удаление из пустой таблицы (создаём новую пустую)
    Table empty;
    init_table(&empty, 5);
    result = erase(&empty, 42);
    assert(result == 1);
    assert(empty.size == 0);
    free_table(&empty);
    printf("Тест 3 пройден\n");

    // Тест 4: удаление единственного элемента
    Table single;
    init_table(&single, 5);
    insert(&single, 100, "hundred");
    result = erase(&single, 100);
    assert(result == 0);
    assert(single.size == 0);
    free_table(&single);
    printf("Тест 4 пройден\n");

    // Тест 5: удаление первого элемента (сдвиг)
    Table first;
    init_table(&first, 5);
    insert(&first, 1, "one");
    insert(&first, 2, "two");
    insert(&first, 3, "three");
    result = erase(&first, 1);
    assert(result == 0);
    assert(first.size == 2);
    assert(first.data[0].key == 2);
    assert(first.data[1].key == 3);
    free_table(&first);
    printf("Тест 5 пройден\n");

    // Тест 6: удаление последнего элемента
    Table last;
    init_table(&last, 5);
    insert(&last, 5, "five");
    insert(&last, 6, "six");
    insert(&last, 7, "seven");
    result = erase(&last, 7);
    assert(result == 0);
    assert(last.size == 2);
    assert(last.data[0].key == 5);
    assert(last.data[1].key == 6);
    free_table(&last);
    printf("Тест 6 пройден\n");

    // Тест 7: удаление из таблицы с capacity = 0
    Table zero_cap;
    init_table(&zero_cap, 0);
    result = erase(&zero_cap, 123);
    assert(result == 1);
    free_table(&zero_cap);
    printf("Тест 7 пройден\n");

    // Тест 8: передача NULL в качестве таблицы
    result = erase(NULL, 10);
    assert(result == 1);
    printf("Тест 8 пройден\n");

    // Тест 9: последовательные вставки и удаления
    Table mixed;
    init_table(&mixed, 10);
    insert(&mixed, 100, "a");
    insert(&mixed, 200, "b");
    insert(&mixed, 300, "c");
    erase(&mixed, 200);
    insert(&mixed, 400, "d");
    erase(&mixed, 100);
    assert(mixed.size == 2);
    assert(mixed.data[0].key == 300);
    assert(mixed.data[1].key == 400);
    free_table(&mixed);
    printf("Тест 9 пройден\n");

    // Тест 10: удаление после удаления (повторное удаление того же ключа)
    Table twice;
    init_table(&twice, 5);
    insert(&twice, 77, "seventy");
    result = erase(&twice, 77);
    assert(result == 0);
    result = erase(&twice, 77);
    assert(result == 1);
    assert(twice.size == 0);
    free_table(&twice);
    printf("Тест 10 пройден\n");

    // Очистка основной таблицы (использовалась в тесте 1)
    free_table(&table);

    printf("Все тесты для функции erase пройдены успешно!\n");
    return 0;
}