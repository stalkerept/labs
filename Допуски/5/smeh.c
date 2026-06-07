#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "graph.h"
#include "list.h"

int shortest_path(const Graph* graph, int from, int to, List* result) {
    // Проверка корректности параметров
    if (!graph || !result ||
        from < 0 || to < 0 ||
        from >= graph->vertex_count || to >= graph->vertex_count)
        return 1;

    int V = graph->vertex_count;
    int **matrix = graph->matrix;
    const long long INF = LLONG_MAX / 2; // условная бесконечность

    // Выделение памяти
    long long *dist = (long long*)malloc(V * sizeof(long long));
    int *prev = (int*)malloc(V * sizeof(int));
    bool *affected = (bool*)calloc(V, sizeof(bool));
    if (!dist || !prev || !affected) {
        free(dist); free(prev); free(affected);
        return 1;
    }

    // Инициализация
    for (int i = 0; i < V; ++i) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[from] = 0;

    // Основной цикл Беллмана – Форда (V-1 итерация)
    for (int i = 0; i < V - 1; ++i) {
        bool updated = false;
        for (int u = 0; u < V; ++u) {
            if (dist[u] == INF) continue;
            for (int v = 0; v < V; ++v) {
                if (matrix[u][v] == 0) continue; // ребра нет
                long long w = matrix[u][v];
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    updated = true;
                }
            }
        }
        if (!updated) break; // досрочный выход
    }

    // Обнаружение вершин, затронутых отрицательными циклами
    for (int u = 0; u < V; ++u) {
        if (dist[u] == INF) continue;
        for (int v = 0; v < V; ++v) {
            if (matrix[u][v] == 0) continue;
            if (dist[u] + matrix[u][v] < dist[v]) {
                affected[v] = true;
            }
        }
    }

    // Распространение флага affected на все достижимые вершины
    bool changed = true;
    while (changed) {
        changed = false;
        for (int u = 0; u < V; ++u) {
            if (!affected[u]) continue;
            for (int v = 0; v < V; ++v) {
                if (matrix[u][v] == 0) continue;
                if (!affected[v]) {
                    affected[v] = true;
                    changed = true;
                }
            }
        }
    }

    // Путь не существует или конечная вершина испорчена отрицательным циклом
    if (dist[to] == INF || affected[to]) {
        free(dist); free(prev); free(affected);
        return 1;
    }

    // Восстановление пути
    int curr = to;
    while (curr != from) {
        List_push_front(result, curr);
        curr = prev[curr];
    }
    List_push_front(result, from);

    // Очистка памяти
    free(dist);
    free(prev);
    free(affected);
    return 0;
}