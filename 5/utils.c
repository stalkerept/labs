#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "structures.h"
#include "func.h"
#include "algorithms.h"
#include "heap.h"
#include "utils.h"


char* read_line(const char* prompt) {
    printf("%s", prompt);
    fflush(stdout);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    if (len == 0) return NULL;
    char* line = malloc(len + 1);
    if (line) strcpy(line, buffer);
    return line;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (scanf("%d", &value) == 1) {
            if (value >= min && value <= max) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                return value;
            }
            printf("Ошибка: число должно быть от %d до %d.\n", min, max);
        } else {
            int c = getchar();
            if (c == EOF) {
                clearerr(stdin);
                printf("\nОбнаружен конец ввода.\n");
                return 0;
            }
            printf("Ошибка ввода. Введите целое число.\n");
            while (c != '\n' && c != EOF) c = getchar();
        }
    }
}

int load_from_file(Graph* graph, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Ошибка открытия файла '%s'\n", filename);
        return -1;
    }
    char line[512];
    int total_edges = 0;
    int errors = 0;
    int line_num = 0;
    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        if (line[0] == '#' || line[0] == '\0') continue;
        char* token1 = strtok(line, " \t");
        if (!token1) { errors++; continue; }
        char* token2 = strtok(NULL, " \t");
        if (!token2) { errors++; continue; }
        char* token3 = strtok(NULL, " \t");
        if (!token3) { errors++; continue; }
        int weight = atoi(token3);
        if (weight < -10 || weight > 10) {
            printf("Строка %d: вес %d вне диапазона [-10,10], пропущена.\n", line_num, weight);
            errors++;
            continue;
        }
        int u = add_vertex(graph, token1);
        int v = add_vertex(graph, token2);
        if (u < 0 || v < 0) {
            printf("Строка %d: ошибка добавления вершин.\n", line_num);
            errors++;
            continue;
        }
        if (add_edge(graph, u, v, weight) != 0) {
            printf("Строка %d: ребро '%s' - '%s' уже существует или ошибка.\n", line_num, token1, token2);
            errors++;
            continue;
        }
        total_edges++;
    }
    fclose(fp);
    printf("Загрузка завершена: добавлено %d рёбер, ошибок в строках: %d\n", total_edges, errors);
    return errors;
}

void handle_add_vertex(Graph* graph) {
    char* name = read_line("Введите имя человека: ");
    if (!name) {
        printf("Некорректное имя.\n");
        return;
    }
    int idx = add_vertex(graph, name);
    if (idx >= 0) {
        printf("Вершина '%s' добавлена (индекс %d).\n", name, idx);
    } else {
        printf("Ошибка: невозможно добавить вершину (возможно, переполнение).\n");
    }
    free(name);
}

void handle_add_edge(Graph* graph) {
    char* name1 = read_line("Имя первого человека: ");
    if (!name1) { printf("Некорректный ввод.\n"); return; }
    char* name2 = read_line("Имя второго человека: ");
    if (!name2) { printf("Некорректный ввод.\n"); free(name1); return; }
    int u = get_vertex_index_by_name(graph, name1);
    int v = get_vertex_index_by_name(graph, name2);
    if (u == -1 || v == -1) {
        printf("Ошибка: один или оба человека не найдены.\n");
        free(name1); free(name2);
        return;
    }
    int weight = read_int("Оценка отношений (от -10 до 10): ", -10, 10);
    if (add_edge(graph, u, v, weight) == 0) {
        printf("Ребро между '%s' и '%s' с весом %d добавлено.\n", name1, name2, weight);
    } else {
        printf("Ошибка: ребро уже существует или некорректные данные.\n");
    }
    free(name1); free(name2);
}

void handle_delete_vertex(Graph* graph) {
    char* name = read_line("Имя удаляемого человека: ");
    if (!name) { printf("Некорректный ввод.\n"); return; }
    if (delete_vertex(graph, name) == 0) {
        printf("Вершина '%s' удалена.\n", name);
    } else {
        printf("Ошибка: вершина не найдена.\n");
    }
    free(name);
}

void handle_delete_edge(Graph* graph) {
    char* name1 = read_line("Имя первого человека: ");
    if (!name1) { printf("Некорректный ввод.\n"); return; }
    char* name2 = read_line("Имя второго человека: ");
    if (!name2) { printf("Некорректный ввод.\n"); free(name1); return; }
    int u = get_vertex_index_by_name(graph, name1);
    int v = get_vertex_index_by_name(graph, name2);
    if (u == -1 || v == -1) {
        printf("Ошибка: вершина не найдена.\n");
        free(name1); free(name2);
        return;
    }
    if (delete_edge(graph, u, v) == 0) {
        printf("Ребро между '%s' и '%s' удалено.\n", name1, name2);
    } else {
        printf("Ошибка при удалении ребра.\n");
    }
    free(name1); free(name2);
}

void handle_change_vertex_name(Graph* graph) {
    char* old_name = read_line("Текущее имя человека: ");
    if (!old_name) { printf("Некорректный ввод.\n"); return; }
    int idx = get_vertex_index_by_name(graph, old_name);
    if (idx == -1) {
        printf("Человек '%s' не найден.\n", old_name);
        free(old_name);
        return;
    }
    char* new_name = read_line("Новое имя: ");
    if (!new_name) { printf("Некорректный ввод.\n"); free(old_name); return; }
    int exist = get_vertex_index_by_name(graph, new_name);
    if (exist != -1 && exist != idx) {
        printf("Ошибка: человек с именем '%s' уже существует.\n", new_name);
        free(old_name); free(new_name);
        return;
    }
    free(graph->vertices[idx].name);
    graph->vertices[idx].name = strdup(new_name);
    if (!graph->vertices[idx].name) {
        printf("Ошибка выделения памяти.\n");
    } else {
        printf("Имя изменено с '%s' на '%s'.\n", old_name, new_name);
    }
    free(old_name); free(new_name);
}

void handle_change_edge_weight(Graph* graph) {
    char* name1 = read_line("Имя первого человека: ");
    if (!name1) return;
    char* name2 = read_line("Имя второго человека: ");
    if (!name2) { free(name1); return; }
    int u = get_vertex_index_by_name(graph, name1);
    int v = get_vertex_index_by_name(graph, name2);
    if (u == -1 || v == -1) {
        printf("Одна из вершин не найдена.\n");
        free(name1); free(name2);
        return;
    }
    AdjNode* edge = NULL;
    for (AdjNode* cur = graph->heads[u]; cur; cur = cur->next) {
        if (cur->dest == v) { edge = cur; break; }
    }
    if (!edge) {
        printf("Ребро между указанными людьми не существует.\n");
        free(name1); free(name2);
        return;
    }
    int new_weight = read_int("Новая оценка отношений (-10..10): ", -10, 10);
    edge->weight = new_weight;
    for (AdjNode* cur = graph->heads[v]; cur; cur = cur->next) {
        if (cur->dest == u) {
            cur->weight = new_weight;
            break;
        }
    }
    printf("Вес ребра '%s' <-> '%s' изменён на %d.\n", name1, name2, new_weight);
    free(name1); free(name2);
}

void handle_print_adjacency(const Graph* graph) {
    printf("\nСписки смежности:\n");
    for (int i = 0; i < graph->len; i++) {
        if (graph->vertices[i].deleted) continue;
        printf("[%s]: ", graph->vertices[i].name);
        AdjNode* cur = graph->heads[i];
        if (!cur) printf("(нет связей)");
        while (cur) {
            printf("-> %s (%d) ", graph->vertices[cur->dest].name, cur->weight);
            cur = cur->next;
        }
        printf("\n");
    }
}

void handle_graphviz(const Graph* graph) {
    FILE* dot = fopen("graph.dot", "w");
    if (!dot) {
        printf("Не удалось создать файл graph.dot\n");
        return;
    }
    fprintf(dot, "graph G {\n");
    for (int i = 0; i < graph->len; i++) {
        if (graph->vertices[i].deleted) continue;
        fprintf(dot, "  \"%s\";\n", graph->vertices[i].name);
    }
    for (int i = 0; i < graph->len; i++) {
        if (graph->vertices[i].deleted) continue;
        for (AdjNode* cur = graph->heads[i]; cur; cur = cur->next) {
            if (i < cur->dest) {
                fprintf(dot, "  \"%s\" -- \"%s\" [label=\"%d\"];\n",
                        graph->vertices[i].name,
                        graph->vertices[cur->dest].name,
                        cur->weight);
            }
        }
    }
    fprintf(dot, "}\n");
    fclose(dot);
    printf("Файл graph.dot создан. Выполните 'dot -Tpng graph.dot -o graph.png'.\n");
}

void handle_dfs(Graph* graph) {
    char* name = read_line("Введите имя человека для поиска потенциальных друзей: ");
    if (!name) return;
    int start = get_vertex_index_by_name(graph, name);
    if (start == -1) {
        printf("Человек не найден.\n");
        free(name);
        return;
    }
    reset_colors(graph);
    printf("Потенциальные друзья (достижимые через неотрицательные отношения):\n");
    dfs(graph, start);
    free(name);
}

void handle_dijkstra(Graph* graph) {
    char* name1 = read_line("Начальный человек: ");
    if (!name1) return;
    char* name2 = read_line("Целевой человек: ");
    if (!name2) { free(name1); return; }
    int u = get_vertex_index_by_name(graph, name1);
    int v = get_vertex_index_by_name(graph, name2);
    if (u == -1 || v == -1) {
        printf("Один или оба человека не найдены.\n");
        free(name1); free(name2);
        return;
    }
    int* dist = (int*)malloc(graph->len * sizeof(int));
    int* prev = (int*)malloc(graph->len * sizeof(int));
    if (!dist || !prev) {
        printf("Ошибка памяти.\n");
        free(dist); free(prev);
        free(name1); free(name2);
        return;
    }
    if (dijkstra(graph, u, dist, prev) != 0) {
        printf("Ошибка выполнения алгоритма Дейкстры.\n");
        free(dist); free(prev);
        free(name1); free(name2);
        return;
    }
    if (dist[v] == INT_MAX) {
        printf("Пути между '%s' и '%s' не существует (только положительные отношения).\n",
               name1, name2);
    } else {
        printf("Кратчайший путь (сумма оценок = %d):\n", dist[v]);
        print_path(graph, prev, v);
        printf("\n");
    }
    free(dist); free(prev);
    free(name1); free(name2);
}

void handle_special_operation(Graph* graph) {
    char* name = read_line("Введите имя человека для поиска наилучшего знакомого: ");
    if (!name) return;
    int start = get_vertex_index_by_name(graph, name);
    if (start == -1) {
        printf("Человек не найден.\n");
        free(name);
        return;
    }
    int** dist = NULL;
    int** next = NULL;
    if (floyd_warshall(graph, &dist, &next) != 0) {
        printf("Ошибка: в графе обнаружен отрицательный цикл.\n");
        if (dist) free_floyd(dist, graph->len);
        if (next) free_floyd(next, graph->len);
        free(name);
        return;
    }
    int n = graph->len;
    int INF = INT_MAX / 2;
    int best_vertex = -1;
    int best_sum = -INF;
    for (int j = 0; j < n; j++) {
        if (j == start) continue;
        if (graph->vertices[j].deleted) continue;
        if (dist[start][j] != INF && dist[start][j] > best_sum) {
            best_sum = dist[start][j];
            best_vertex = j;
        }
    }
    if (best_vertex == -1) {
        printf("Нет достижимых потенциальных знакомых для '%s'.\n", graph->vertices[start].name);
    } else {
        printf("Наибольшая сумма оценок кратчайшего пути: %d\n", best_sum);
        printf("Цепочка: ");
        print_path_floyd(graph, next, start, best_vertex);
    }
    free_floyd(dist, n);
    if (next) free_floyd(next, n);
    free(name);
}

void handle_load_from_file(Graph* graph) {
    char* filename = read_line("Введите имя файла: ");
    if (!filename) {
        printf("Некорректный ввод имени файла.\n");
        return;
    }
    load_from_file(graph, filename);
    free(filename);
}

void show_menu() {
    printf("\n===== Меню =====\n");
    printf("1. Добавить вершину\n");
    printf("2. Добавить ребро\n");
    printf("3. Удалить вершину\n");
    printf("4. Удалить ребро\n");
    printf("5. Изменить имя вершины\n");
    printf("6. Изменить вес ребра\n");
    printf("7. Вывести списки смежности\n");
    printf("8. Графический вывод (Graphviz)\n");
    printf("9. Обход (потенциальные друзья)\n");
    printf("10. Кратчайший путь (Дейкстра)\n");
    printf("11. Особая операция (Флойд-Уоршелл)\n");
    printf("12. Загрузить граф из файла\n");
    printf("13. Выход\n");
    printf("Выберите действие: ");
}