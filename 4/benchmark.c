// benchmark.c
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include "func.h"
#include "utils.h"

// Генерация ключей и значений
static void random_key(char *buf, size_t len, int seed) {
    snprintf(buf, len, "key%08d", seed);
}
static void random_info(char *buf, size_t len, int seed) {
    snprintf(buf, len, "value%08d", seed);
}

// Точное время
static long long get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

// Обход без вывода
static void traverse_silent(Tree *tree) {
    if (!tree || !tree->root) return;
    Stack* stack = stack_init(tree->count);
    if (!stack) return;
    Node *curr = tree->root;
    long long count = 0;
    while (curr || stack->top != -1) {
        if (curr) {
            stack->array[++stack->top] = curr;
            curr = curr->right;
        } else {
            curr = stack->array[stack->top--];
            count++;
            curr = curr->left;
        }
    }
    free(stack->array);
    free(stack);
    (void)count;
}

// Медиана
static int compare_double(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}
static double median(double *arr, int n) {
    qsort(arr, n, sizeof(double), compare_double);
    return (n % 2) ? arr[n/2] : (arr[n/2 - 1] + arr[n/2]) / 2.0;
}

// Основная функция замера для одного N
static BenchmarkPoint benchmark_size(int N, int experiments) {
    BenchmarkPoint result = {N, 0.0, 0.0, 0.0, 0.0, 0.0};

    // Параметры повторений
    const int SEARCH_REPEATS = (N < 50000) ? 5000 : 10000;  // больше повторений для больших N
    const int TO_DELETE = N / 20;  // удаляем 5% элементов, чтобы дерево не сильно менялось
    if (TO_DELETE < 1) return result;

    double *insert_times  = malloc(experiments * sizeof(double));
    double *delete_times  = malloc(experiments * sizeof(double));
    double *find_times    = malloc(experiments * sizeof(double));
    double *special_times = malloc(experiments * sizeof(double));
    double *traverse_times= malloc(experiments * sizeof(double));

    for (int exp = 0; exp < experiments; exp++) {
        Tree *tree = tree_init();
        if (!tree) { fprintf(stderr, "Memory error\n"); exit(1); }

        // Генерация всех ключей
        char (*keys)[32] = malloc(N * sizeof(char[32]));
        char (*infos)[32] = malloc(N * sizeof(char[32]));
        for (int i = 0; i < N; i++) {
            random_key(keys[i], 32, i);
            random_info(infos[i], 32, i);
        }

        // Перемешивание порядка вставки
        int *order = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) order[i] = i;
        for (int i = N - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = order[i]; order[i] = order[j]; order[j] = tmp;
        }

        // 1. ВСТАВКА
        long long insert_total_ns = 0;
        for (int i = 0; i < N; i++) {
            int idx = order[i];
            long long start = get_time_ns();
            char *old = insert(tree, keys[idx], infos[idx]);
            long long end = get_time_ns();
            insert_total_ns += (end - start);
            if (old) free(old);
        }
        free(order);
        insert_times[exp] = (insert_total_ns / (double)N) / 1000.0;

        // 2. ПОИСК
        long long find_total_ns = 0;
        for (int r = 0; r < SEARCH_REPEATS; r++) {
            int idx = rand() % N;
            long long start = get_time_ns();
            Node *res = find(tree, keys[idx]);
            long long end = get_time_ns();
            find_total_ns += (end - start);
            (void)res;
        }
        find_times[exp] = (find_total_ns / (double)SEARCH_REPEATS) / 1000.0;

        // 3. СПЕЦИАЛЬНЫЙ ПОИСК
        long long special_total_ns = 0;
        for (int r = 0; r < SEARCH_REPEATS; r++) {
            int idx = rand() % N;
            long long start = get_time_ns();
            Node *res = find_special(tree, keys[idx]);
            long long end = get_time_ns();
            special_total_ns += (end - start);
            (void)res;
        }
        special_times[exp] = (special_total_ns / (double)SEARCH_REPEATS) / 1000.0;

        // 4. ОБХОД (прогрев + замер)
        traverse_silent(tree);
        long long trav_start = get_time_ns();
        traverse_silent(tree);
        long long trav_end = get_time_ns();
        traverse_times[exp] = (trav_end - trav_start) / 1000.0;

        // 5. УДАЛЕНИЕ (случайные TO_DELETE ключей)
        int *del_indices = malloc(TO_DELETE * sizeof(int));
        for (int i = 0; i < TO_DELETE; i++) del_indices[i] = i;
        for (int i = TO_DELETE - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = del_indices[i];
            del_indices[i] = del_indices[j];
            del_indices[j] = tmp;
        }
        long long delete_total_ns = 0;
        for (int i = 0; i < TO_DELETE; i++) {
            char *key_to_del = keys[del_indices[i]];
            long long start = get_time_ns();
            delete(tree, key_to_del);
            long long end = get_time_ns();
            delete_total_ns += (end - start);
        }
        delete_times[exp] = (delete_total_ns / (double)TO_DELETE) / 1000.0;

        free(del_indices);
        free(keys);
        free(infos);
        free_tree(tree);
        free(tree);
    }

    // Медиана по экспериментам
    result.insert_time_us   = median(insert_times, experiments);
    result.delete_time_us   = median(delete_times, experiments);
    result.find_time_us     = median(find_times, experiments);
    result.special_time_us  = median(special_times, experiments);
    result.traverse_time_us = median(traverse_times, experiments);

    free(insert_times);
    free(delete_times);
    free(find_times);
    free(special_times);
    free(traverse_times);

    return result;
}

int main(void) {
    srand(time(NULL));

    // Размеры для замеров (можно настроить)
    int sizes[] = {100000, 200000, 300000, 400000, 500000,
                   600000, 700000, 800000, 900000, 1000000, 1100000, 1200000, 1300000,
                    1400000, 1500000,
                   1600000, 1700000, 1800000, 1900000, 2000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    const int EXPERIMENTS = 15;  // больше экспериментов для стабильности

    printf("size,insert_us,delete_us,find_us,special_us,traverse_us\n");

    for (int s = 0; s < num_sizes; s++) {
        int N = sizes[s];
        BenchmarkPoint pt = benchmark_size(N, EXPERIMENTS);
        printf("%d,%.3f,%.3f,%.3f,%.3f,%.3f\n",
               pt.size,
               pt.insert_time_us,
               pt.delete_time_us,
               pt.find_time_us,
               pt.special_time_us,
               pt.traverse_time_us);
        fflush(stdout);
    }

    return 0;
}