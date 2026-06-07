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
    Node** data;           // массив указателей на головы списков
    int capacity;          // размер массива
    unsigned long long (*hash)(char*); // хеш-функция
} Table;

// ---------- Вспомогательные функции для тестов ----------

// Простая хеш-функция для тестов: сумма символов по модулю capacity
unsigned long long simple_hash(char* key) {
    unsigned long long h = 0;
    for (char* p = key; *p; p++)
        h += (unsigned char)(*p);
    return h;
}

// Создание таблицы с заданной capacity и хеш-функцией
Table* create_table(int capacity, unsigned long long (*hash)(char*)) {
    Table* t = (Table*)malloc(sizeof(Table));
    t->capacity = capacity;
    t->hash = hash;
    t->data = (Node**)calloc(capacity, sizeof(Node*));
    return t;
}

// Освобождение таблицы и всей памяти
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

// Поиск ключа в таблице (возвращает 1, если найден)
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

// Подсчёт количества элементов в таблице
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

// int errace(Table* table, char* key){
//     if(!table || !(table -> data) || !(table->hash) || !key) return 1;
//     unsigned long long k = table -> hash(key);
//     Node* n = table -> data[k];
//     Node* p = NULL;
//     while(n && n->key == key){
//         p = n;
//         n = n->next;
//     }
//     if(!n) return 1;
//     if(p == NULL){
//         table -> data[k] = n->next;
//     }
//     else{
//         p -> next = n -> next;
//     }
//     free(n -> info);
//     free(n);
//     return 0;
// }
int insert(Table* table, char* key, char* info){
    if(!table || !(table -> data) || !(table->hash) || !info || !key) return 1;
    unsigned long long k = (table -> hash(key))%(table->capacity);
    char* new_info = strdup(info);
    if(!new_info) return 1;
    char* new_key = strdup(key);
    if(!new_key){
        free(new_info);
        return 1;
    }
    Node* new = (Node*)malloc(sizeof(Node));
    if(!new){
        free(new_info);
        free(new_key);
        return 1;
    }
    new -> key = new_key;
    new -> info = new_info;
    new -> next = NULL;
    Node* n = table -> data[k];
    if(!n){
        table -> data[k] = new;
    }
    else{
        while(n){
            if(strcmp(n -> key, key) == 0){
                free(new_info);
                free(new_key);
                free(new);
                return 1;
            }
            n = n->next;
        }    
        new -> next = table -> data[k];
        table -> data[k] = new;
    }
    return 0;
}

// ---------- Основные тесты ----------

int main() {
    // Тест 1: вставка в пустую таблицу
    Table* t1 = create_table(5, simple_hash);
    int res = insert(t1, "one", "один");
    assert(res == 0);
    assert(count_elements(t1) == 1);
    assert(find_key(t1, "one"));
    printf("Тест 1 пройден\n");
    free_table(t1);

    // Тест 2: вставка второго элемента с другим ключом
    Table* t2 = create_table(5, simple_hash);
    insert(t2, "one", "один");
    res = insert(t2, "two", "два");
    assert(res == 0);
    assert(count_elements(t2) == 2);
    assert(find_key(t2, "one") && find_key(t2, "two"));
    printf("Тест 2 пройден\n");
    free_table(t2);

    // Тест 3: попытка вставить дубликат ключа
    Table* t3 = create_table(5, simple_hash);
    insert(t3, "same", "первый");
    res = insert(t3, "same", "дубликат");
    assert(res == 1);
    assert(count_elements(t3) == 1);
    // Проверяем, что исходное значение не изменилось
    unsigned long long idx = simple_hash("same") % 5;
    Node* cur = t3->data[idx];
    while (cur && strcmp(cur->key, "same") != 0) cur = cur->next;
    assert(cur && strcmp(cur->info, "первый") == 0);
    printf("Тест 3 пройден\n");
    free_table(t3);

    // Тест 4: вставка с NULL key или NULL info
    Table* t4 = create_table(5, simple_hash);
    res = insert(t4, NULL, "info");
    assert(res == 1);
    res = insert(t4, "key", NULL);
    assert(res == 1);
    assert(count_elements(t4) == 0);
    printf("Тест 4 пройден\n");
    free_table(t4);

    // Тест 5: вставка элементов, вызывающих коллизии
    Table* t5 = create_table(3, simple_hash);
    // хеш-функция simple_hash даёт коллизию для "ab" и "ba" (сумма одинакова)
    insert(t5, "ab", "first");
    insert(t5, "ba", "second");
    insert(t5, "ac", "third"); // другой хеш
    assert(count_elements(t5) == 3);
    assert(find_key(t5, "ab") && find_key(t5, "ba") && find_key(t5, "ac"));
    // Проверяем, что оба элемента с коллизией лежат в одной цепочке
    unsigned long long idx_ab = simple_hash("ab") % 3;
    int chain_len = 0;
    Node* cur5 = t5->data[idx_ab];
    while (cur5) {
        chain_len++;
        cur5 = cur5->next;
    }
    assert(chain_len == 2); // "ab" и "ba" должны быть в одной цепочке
    printf("Тест 5 пройден\n");
    free_table(t5);

    // Тест 6: передача NULL таблицы
    res = insert(NULL, "key", "info");
    assert(res == 1);
    printf("Тест 6 пройден\n");

    // Тест 7: корректность после неудачной вставки (состояние не изменилось)
    Table* t7 = create_table(5, simple_hash);
    insert(t7, "k1", "v1");
    insert(t7, "k2", "v2");
    res = insert(t7, "k1", "new");
    assert(res == 1);
    assert(count_elements(t7) == 2);
    assert(find_key(t7, "k1") && find_key(t7, "k2"));
    // Проверяем, что info для k1 осталось прежним
    unsigned long long idx7 = simple_hash("k1") % 5;
    Node* cur7 = t7->data[idx7];
    while (cur7 && strcmp(cur7->key, "k1") != 0) cur7 = cur7->next;
    assert(cur7 && strcmp(cur7->info, "v1") == 0);
    printf("Тест 7 пройден\n");
    free_table(t7);

    // Тест 8: вставка большого количества элементов (проверка отсутствия утечек)
    Table* t8 = create_table(10, simple_hash);
    char keys[100][10];
    for (int i = 0; i < 50; i++) {
        sprintf(keys[i], "key%d", i);
        res = insert(t8, keys[i], "value");
        assert(res == 0);
    }
    assert(count_elements(t8) == 50);
    for (int i = 0; i < 50; i++) {
        assert(find_key(t8, keys[i]));
    }
    printf("Тест 8 пройден\n");
    free_table(t8);

    // Тест 9: проверка копирования строк (чтобы таблица не зависела от внешних изменений)
    Table* t9 = create_table(5, simple_hash);
    char* ext_key = strdup("dynamic_key");
    char* ext_info = strdup("dynamic_info");
    res = insert(t9, ext_key, ext_info);
    assert(res == 0);
    // Изменяем внешние строки
    strcpy(ext_key, "changed");
    strcpy(ext_info, "changed");
    // В таблице должно остаться исходное содержимое
    unsigned long long idx9 = simple_hash("dynamic_key") % 5;
    Node* cur9 = t9->data[idx9];
    assert(cur9 && strcmp(cur9->key, "dynamic_key") == 0);
    assert(strcmp(cur9->info, "dynamic_info") == 0);
    free(ext_key);
    free(ext_info);
    free_table(t9);
    printf("Тест 9 пройден\n");

    // Тест 10: вставка пустой строки (должна работать)
    Table* t10 = create_table(5, simple_hash);
    res = insert(t10, "", "empty_key");
    assert(res == 0);
    assert(find_key(t10, ""));
    res = insert(t10, "key", "");
    assert(res == 0);
    assert(find_key(t10, "key"));
    Node* n10 = t10->data[simple_hash("") % 5];
    while (n10 && strcmp(n10->key, "") != 0) n10 = n10->next;
    assert(n10 && strcmp(n10->info, "empty_key") == 0);
    printf("Тест 10 пройден\n");
    free_table(t10);

    printf("Все тесты для функции insert пройдены успешно!\n");
    return 0;
}
