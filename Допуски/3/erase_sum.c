#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* key;
    char* info;
    int busy;   /* 0 - free, 1 - busy, 2 - deleted */
} Node;

typedef struct Table {
    Node* data;
    int capacity;
    int step;
    unsigned long long (*hash)(char*);
} Table;

/* Твоя функция */
int erase(Table* table, char* key){
    if (!table || !key || table -> capacity <= 0 || !(table -> data) || (table -> step <= 0) || !(table -> hash)) return 1;
    unsigned long long k = table -> hash(key) % table -> capacity, ind = 0;
    Node* ptr = &(table -> data[k]);
    for(int step = 0; step<table->capacity; step++){
        ind = (k + step * table -> step)%(table -> capacity);
        ptr = &(table -> data[ind]);
        if(ptr -> busy == 1 && ptr->key != NULL && strcmp(key, ptr->key) == 0){
            ptr->busy = 2;
            return 0;
        }
        else if(ptr -> busy == 0) return 1;
    }
    return 1;
}
/* ===================================================== */
/*                    ВСПОМОГАТЕЛЬНОЕ                    */
/* ===================================================== */

static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(expr) do { \
    if (!(expr)) { \
        printf("    [FAIL] %s:%d: %s\n", __FILE__, __LINE__, #expr); \
        return 0; \
    } \
} while (0)

#define ASSERT_INT_EQ(expected, actual) do { \
    int e_ = (expected); \
    int a_ = (actual); \
    if (e_ != a_) { \
        printf("    [FAIL] %s:%d: expected %d, got %d\n", __FILE__, __LINE__, e_, a_); \
        return 0; \
    } \
} while (0)

#define ASSERT_STR_EQ(expected, actual) do { \
    const char* e_ = (expected); \
    const char* a_ = (actual); \
    if ((e_ == NULL && a_ != NULL) || (e_ != NULL && a_ == NULL) || (e_ && a_ && strcmp(e_, a_) != 0)) { \
        printf("    [FAIL] %s:%d: expected \"%s\", got \"%s\"\n", \
               __FILE__, __LINE__, e_ ? e_ : "NULL", a_ ? a_ : "NULL"); \
        return 0; \
    } \
} while (0)

static char* my_strdup(const char* s) {
    size_t n = strlen(s) + 1;
    char* p = (char*)malloc(n);
    if (!p) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    memcpy(p, s, n);
    return p;
}

static unsigned long long hash_sum(char* s) {
    unsigned long long h = 0;
    while (*s) {
        h = h * 131 + (unsigned char)(*s);
        s++;
    }
    return h;
}

/* Плохой хеш: всегда одна стартовая позиция */
static unsigned long long bad_hash(char* s) {
    (void)s;
    return 0ULL;
}

static Table make_table(int capacity, int step, unsigned long long (*hash_fn)(char*)) {
    Table t;
    t.capacity = capacity;
    t.step = step;
    t.hash = hash_fn;
    t.data = (Node*)calloc((size_t)capacity, sizeof(Node));
    if (!t.data) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    return t;
}

static void free_table(Table* t) {
    if (!t || !t->data) return;

    for (int i = 0; i < t->capacity; i++) {
        if ((t->data[i].busy == 1 || t->data[i].busy == 2) && t->data[i].key) {
            free(t->data[i].key);
            free(t->data[i].info);
            t->data[i].key = NULL;
            t->data[i].info = NULL;
        }
    }

    free(t->data);
    t->data = NULL;
    t->capacity = 0;
    t->step = 0;
    t->hash = NULL;
}

/* Ручная установка ячейки для тестов */
static void set_slot(Table* t, int index, const char* key, const char* info, int busy) {
    t->data[index].key = key ? my_strdup(key) : NULL;
    t->data[index].info = info ? my_strdup(info) : NULL;
    t->data[index].busy = busy;
}

/* Линейный поиск по массиву — только для проверки тестов */
static int find_key_anywhere(Table* t, const char* key) {
    for (int i = 0; i < t->capacity; i++) {
        if (t->data[i].busy == 1 && t->data[i].key && strcmp(t->data[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

static void run_test(const char* name, int (*test_fn)(void)) {
    tests_run++;
    printf("[TEST] %s\n", name);
    if (test_fn()) {
        printf("    [OK]\n");
    } else {
        tests_failed++;
    }
}

/* ===================================================== */
/*                         ТЕСТЫ                         */
/* ===================================================== */

/* 1. Некорректные аргументы */
static int test_invalid_args(void) {
    Table t = make_table(7, 3, hash_sum);

    ASSERT_INT_EQ(1, erase(NULL, "x"));
    ASSERT_INT_EQ(1, erase(&t, NULL));

    free_table(&t);
    return 1;
}

/* 2. Удаление существующего элемента из его домашней позиции */
static int test_erase_existing_home_slot(void) {
    Table t = make_table(7, 3, bad_hash);

    /* bad_hash -> стартовый индекс 0 */
    set_slot(&t, 0, "aa", "info_aa", 1);

    ASSERT_INT_EQ(0, erase(&t, "aa"));

    ASSERT_INT_EQ(2, t.data[0].busy);
    ASSERT_TRUE(find_key_anywhere(&t, "aa") == -1);

    free_table(&t);
    return 1;
}

/* 3. Попытка удалить отсутствующий ключ */
static int test_erase_missing_key(void) {
    Table t = make_table(7, 3, bad_hash);

    set_slot(&t, 0, "aa", "info_aa", 1);

    ASSERT_INT_EQ(1, erase(&t, "bb"));

    ASSERT_INT_EQ(1, t.data[0].busy);
    ASSERT_TRUE(find_key_anywhere(&t, "aa") == 0);

    free_table(&t);
    return 1;
}

/* 4. Удаление элемента после коллизии по шагу сложения
   При bad_hash все стартуют с 0.
   При step = 3 последовательность: 0 -> 3 -> 6 -> 2 -> ...
*/
static int test_erase_after_collision(void) {
    Table t = make_table(7, 3, bad_hash);

    set_slot(&t, 0, "aa", "info_aa", 1);
    set_slot(&t, 3, "bb", "info_bb", 1);
    set_slot(&t, 6, "cc", "info_cc", 1);

    ASSERT_INT_EQ(0, erase(&t, "bb"));

    ASSERT_INT_EQ(1, t.data[0].busy);
    ASSERT_INT_EQ(2, t.data[3].busy);
    ASSERT_INT_EQ(1, t.data[6].busy);

    ASSERT_TRUE(find_key_anywhere(&t, "aa") == 0);
    ASSERT_TRUE(find_key_anywhere(&t, "bb") == -1);
    ASSERT_TRUE(find_key_anywhere(&t, "cc") == 6);

    free_table(&t);
    return 1;
}

/* 5. Проверка, что удалённая ячейка (busy=2) не останавливает поиск.
   Иначе erase("cc") может ошибочно решить, что элемента нет.
*/
static int test_deleted_slot_does_not_stop_search(void) {
    Table t = make_table(7, 3, bad_hash);

    set_slot(&t, 0, "aa", "info_aa", 1);
    set_slot(&t, 3, NULL, NULL, 2);      /* tombstone */
    set_slot(&t, 6, "cc", "info_cc", 1);

    ASSERT_INT_EQ(0, erase(&t, "cc"));

    ASSERT_INT_EQ(1, t.data[0].busy);
    ASSERT_INT_EQ(2, t.data[3].busy);
    ASSERT_INT_EQ(2, t.data[6].busy);

    ASSERT_TRUE(find_key_anywhere(&t, "cc") == -1);

    free_table(&t);
    return 1;
}

/* 6. Попытка удалить из пустой таблицы */
static int test_erase_from_empty_table(void) {
    Table t = make_table(7, 3, hash_sum);

    ASSERT_INT_EQ(1, erase(&t, "ghost"));

    for (int i = 0; i < t.capacity; i++) {
        ASSERT_INT_EQ(0, t.data[i].busy);
    }

    free_table(&t);
    return 1;
}

/* ===================================================== */
/*                          MAIN                         */
/* ===================================================== */

int main(void) {
    run_test("invalid arguments", test_invalid_args);
    run_test("erase existing element from home slot", test_erase_existing_home_slot);
    run_test("erase missing key", test_erase_missing_key);
    run_test("erase element after collision", test_erase_after_collision);
    run_test("deleted slot does not stop search", test_deleted_slot_does_not_stop_search);
    run_test("erase from empty table", test_erase_from_empty_table);

    printf("\n============================\n");
    printf("Tests run:    %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);
    printf("============================\n");

    return (tests_failed == 0) ? 0 : 1;
}