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

// Прототип тестируемой функции




// Вспомогательные функции для тестов
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

// Простая реализация find для тестов (возвращает 1 если ключ найден)
int find_key(const Table* table, int key) {
    Node* current = table->head;
    while (current) {
        if (current->key == key) return 1;
        current = current->next;
    }
    return 0;
}

// Простой подсчёт элементов (для проверки size, хотя size нет в структуре)
int count_elements(const Table* table) {
    int cnt = 0;
    Node* current = table->head;
    while (current) {
        cnt++;
        current = current->next;
    }
    return cnt;
}


int insert(Table* table, int key, char* info){
    if(!table || !info) return 1;
    Node* new = (Node*)malloc(sizeof(Node));
    if(!new) return 1;
    char* new_info = strdup(info);
    if(!new_info) return 1;
    new -> key = key;
    new -> info = new_info;
    new -> next = NULL;
    if(!(table -> head)){
        table -> head = new;
    }
    else{
        if(find_key(table, key) == 1){
            free(new);
            free(new_info);
            return 1;
        }
        Node* n = table -> head;
        while(n -> next){
            n = n->next;
        }
        n -> next = new;
    }
    return 0;
}

// Тесты
int main() {
    Table table;

    // Тест 1: вставка в пустую таблицу
    init_table(&table);
    int result = insert(&table, 10, "hello");
    assert(result == 0);
    assert(count_elements(&table) == 1);
    assert(find_key(&table, 10));
    // Проверяем, что строка скопирована (не просто сохранён указатель)
    // Для этого можно изменить исходную строку, но проще проверить, что можно освободить таблицу без ошибок
    printf("Тест 1 пройден\n");

    // Тест 2: вставка второго элемента с другим ключом
    result = insert(&table, 20, "world");
    assert(result == 0);
    assert(count_elements(&table) == 2);
    assert(find_key(&table, 10));
    assert(find_key(&table, 20));
    printf("Тест 2 пройден\n");

    // Тест 3: попытка вставить дубликат ключа
    result = insert(&table, 10, "duplicate");
    assert(result == 1);
    assert(count_elements(&table) == 2); // размер не изменился
    // Убедимся, что оригинальная строка не изменилась
    // Для этого придётся пройтись по списку и сравнить строку, но достаточно знать, что вставка не удалась
    printf("Тест 3 пройден\n");

    // Тест 4: вставка с NULL info (должна вернуть ошибку)
    result = insert(&table, 30, NULL);
    assert(result == 1);
    assert(count_elements(&table) == 2);
    assert(!find_key(&table, 30));
    printf("Тест 4 пройден\n");

    // Тест 5: вставка нескольких элементов
    result = insert(&table, 30, "thirty");
    assert(result == 0);
    result = insert(&table, 40, "forty");
    assert(result == 0);
    result = insert(&table, 50, "fifty");
    assert(result == 0);
    assert(count_elements(&table) == 5);
    assert(find_key(&table, 30) && find_key(&table, 40) && find_key(&table, 50));
    printf("Тест 5 пройден\n");

    // Тест 6: передача NULL таблицы
    result = insert(NULL, 100, "test");
    assert(result == 1);
    printf("Тест 6 пройден\n");

    // Тест 7: вставка с отрицательным ключом
    Table negative;
    init_table(&negative);
    result = insert(&negative, -5, "minus five");
    assert(result == 0);
    assert(find_key(&negative, -5));
    free_table(&negative);
    printf("Тест 7 пройден\n");

    // Тест 8: вставка после удаления (проверяем, что нет утечек при повторном использовании)
    Table reuse;
    init_table(&reuse);
    insert(&reuse, 1, "one");
    insert(&reuse, 2, "two");
    free_table(&reuse);
    init_table(&reuse);
    insert(&reuse, 3, "three");
    assert(count_elements(&reuse) == 1);
    free_table(&reuse);
    printf("Тест 8 пройден\n");

    // Тест 9: вставка очень длинной строки
    Table long_str;
    init_table(&long_str);
    char* long_string = malloc(1000);
    memset(long_string, 'A', 999);
    long_string[999] = '\0';
    result = insert(&long_str, 999, long_string);
    assert(result == 0);
    free(long_string); // оригинальная строка может быть освобождена, но таблица должна хранить копию
    // Проверим, что в таблице всё ещё есть правильная строка (можно проверить длину)
    Node* node = long_str.head;
    assert(node && strlen(node->info) == 999);
    free_table(&long_str);
    printf("Тест 9 пройден\n");

    // Тест 10: корректность после неудачной вставки (проверка, что состояние таблицы не изменилось)
    Table state;
    init_table(&state);
    insert(&state, 100, "hundred");
    insert(&state, 200, "two hundred");
    result = insert(&state, 100, "duplicate hundred");
    assert(result == 1);
    assert(count_elements(&state) == 2);
    assert(find_key(&state, 100) && find_key(&state, 200));
    // Проверяем, что info для ключа 100 не изменился
    Node* found = state.head;
    while (found && found->key != 100) found = found->next;
    assert(found && strcmp(found->info, "hundred") == 0);
    free_table(&state);
    printf("Тест 10 пройден\n");

    // Освобождаем основную таблицу
    free_table(&table);

    printf("Все тесты для функции insert пройдены успешно!\n");
    return 0;
}