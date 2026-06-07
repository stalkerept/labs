#include "graph.h"
#include "list.h"
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

int shortest_path(const Graph* graph, int from, int to, List* result) {
    // Проверка корректности параметров
    if (graph == NULL || result == NULL) return 1;
    if (from < 0 || from >= graph->vertex_count ||
        to < 0   || to   >= graph->vertex_count) return 1;

    int V = graph->vertex_count;

    // Выделение памяти под вспомогательные массивы
    long long* dist = (long long*)malloc(V * sizeof(long long));
    int*       pred = (int*)      malloc(V * sizeof(int));
    bool*    affected = (bool*)   malloc(V * sizeof(bool));
    if (dist == NULL || pred == NULL || affected == NULL) {
        free(dist);
        free(pred);
        free(affected);
        return 1;
    }

    const long long INF = LLONG_MAX / 2;  // во избежание переполнения при сложении
    for (int i = 0; i < V; ++i) {
        dist[i] = INF;
        pred[i] = -1;
        affected[i] = false;
    }
    dist[from] = 0;

    // Алгоритм Беллмана – Форда (V-1 итерация)
    for (int i = 0; i < V - 1; ++i) {
        bool updated = false;
        for (int u = 0; u < V; ++u) {
            if (dist[u] == INF) continue;
            Vertex* vertex = &graph->vertices[u];
            for (int j = 0; j < vertex->edge_count; ++j) {
                Edge e = vertex->edges[j];
                int v = e.to;
                long long w = e.weight;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pred[v] = u;
                    updated = true;
                }
            }
        }
        if (!updated) break;   // досрочный выход, если релаксаций больше нет
    }

    // Поиск вершин, затронутых отрицательными циклами, достижимыми из from
    for (int u = 0; u < V; ++u) {
        if (dist[u] == INF) continue;
        Vertex* vertex = &graph->vertices[u];
        for (int j = 0; j < vertex->edge_count; ++j) {
            Edge e = vertex->edges[j];
            if (dist[u] + e.weight < dist[e.to]) {
                affected[e.to] = true;
            }
        }
    }

    // Распространение флага affected на все достижимые из этих вершин
    bool changed = true;
    while (changed) {
        changed = false;
        for (int u = 0; u < V; ++u) {
            if (!affected[u]) continue;
            Vertex* vertex = &graph->vertices[u];
            for (int j = 0; j < vertex->edge_count; ++j) {
                int v = vertex->edges[j].to;
                if (!affected[v]) {
                    affected[v] = true;
                    changed = true;
                }
            }
        }
    }

    // Если конечная вершина затронута отрицательным циклом – ошибка
    if (affected[to]) {
        free(dist); free(pred); free(affected);
        return 1;
    }

    // Если пути не существует – ошибка
    if (dist[to] == INF) {
        free(dist); free(pred); free(affected);
        return 1;
    }

    // Восстановление пути (вершины от from до to)
    int curr = to;
    while (curr != from) {
        List_push_front(result, curr);
        curr = pred[curr];
        if (curr == -1) {   // не должно произойти, если путь существует
            free(dist); free(pred); free(affected);
            return 1;
        }
    }
    List_push_front(result, from);

    // Освобождение памяти
    free(dist);
    free(pred);
    free(affected);
    return 0;
}