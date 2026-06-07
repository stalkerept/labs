#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "heap.h"
#include <limits.h>
#include <stdio.h>

void reset_colors(Graph* graph){
    for (int i = 0; i < graph->len; i++) {
        if (!graph->vertices[i].deleted)
            graph->vertices[i].color = WHITE;
    }
    return;
}

void dfs(Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->len) return;
    if (graph->vertices[start].deleted) return;
    int* stack = (int*)malloc(sizeof(int) * graph->len);
    if (!stack) return;
    int top = -1;
    stack[++top] = start;
    while (top >= 0) {
        int u = stack[top--];
        if (graph->vertices[u].deleted || graph->vertices[u].color != WHITE)
            continue;
        graph->vertices[u].color = GRAY;
        printf("Visit: %s\n", graph->vertices[u].name);
        AdjNode* cur = graph->heads[u];
        while (cur) {
            int v = cur->dest;
            if (!graph->vertices[v].deleted && graph->vertices[v].color == WHITE && cur -> weight >= 0) {
                stack[++top] = v;
            }
            cur = cur->next;
        }
        graph->vertices[u].color = BLACK;
    }
    free(stack);
    return;
}

void dfs_full(Graph* graph) {
    reset_colors(graph);
    for (int i = 0; i < graph->len; i++) {
        if (!graph->vertices[i].deleted && graph->vertices[i].color == WHITE) {
            dfs(graph, i);
        }
    }
    return;
}

int dijkstra(Graph* graph, int start, int* dist, int* prev) {
    if (!graph || !graph -> vertices || !graph -> heads || !dist) return 1;
    if (start < 0 || start >= graph->len || graph->vertices[start].deleted) return 1;
    int n = graph->len;
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        if (prev) prev[i] = -1;
    }
    dist[start] = 0;
    MinHeap* heap = init_heap(n * 2);
    if (!heap) return 1;
    heap_push(heap, start, 0);
    while (heap->size > 0) {
        int u, d;
        if (!heap_pop(heap, &u, &d)) break;
        if (d != dist[u]) continue;
        AdjNode* cur = graph->heads[u];
        while (cur) {
            int v = cur->dest;
            int w = cur->weight;
            if (!graph->vertices[v].deleted && w >= 0 && dist[u] != INT_MAX) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    if (prev) prev[v] = u;
                    heap_push(heap, v, dist[v]);
                }
            }
            cur = cur->next;
        }
    }
    free_heap(heap);
    return 0;
}

void print_path(Graph* graph, int* prev, int target) {
    if (target < 0 || target >= graph->len || graph->vertices[target].deleted) {
        printf("Некорректная вершина\n");
        return;
    }
    if (prev[target] == -1) {
        printf("%s", graph->vertices[target].name);
        return;
    }
    int path[graph->len];
    int len = 0;
    int curr = target;
    while (curr != -1) {
        path[len++] = curr;
        curr = prev[curr];
    }
    for (int i = len - 1; i >= 0; i--) {
        printf("%s ", graph->vertices[path[i]].name);
    }
    printf("\n");
}

int floyd_warshall(Graph* graph, int*** dist_out, int*** next_out) {
    int n = graph->len;
    int INF = INT_MAX/2;
    int** dist = (int**)malloc(n * sizeof(int*));
    if (!dist) {
        *dist_out = NULL;
        if (next_out) *next_out = NULL;
        return -1;
    }
    int** next = NULL;
    if (next_out) {
        next = (int**)malloc(n * sizeof(int*));
        if (!next) {
            free(dist);
            *dist_out = NULL;
            *next_out = NULL;
            return -1;
        }
    }
    for (int i = 0; i < n; i++) {
        dist[i] = (int*)malloc(n * sizeof(int));
        if (!dist[i]) {
            for (int j = 0; j < i; j++) free(dist[j]);
            free(dist);
            if (next) {
                for (int j = 0; j < i; j++) free(next[j]);
                free(next);
            }
            *dist_out = NULL;
            if (next_out) *next_out = NULL;
            return -1;
        }
        if (next) {
            next[i] = (int*)malloc(n * sizeof(int));
            if (!next[i]) {
                for (int j = 0; j <= i; j++) free(dist[j]);
                free(dist);
                for (int j = 0; j < i; j++) free(next[j]);
                free(next);
                *dist_out = NULL;
                *next_out = NULL;
                return -1;
            }
        }
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
                if (next) next[i][j] = -1;
            } else {
                dist[i][j] = INF;
                if (next) next[i][j] = -1;
            }
        }
    }
    for (int u = 0; u < n; u++) {
        if (graph->vertices[u].deleted) continue;
        AdjNode* cur = graph->heads[u];
        while (cur) {
            int v = cur->dest;
            int w = cur->weight;
            if (!graph->vertices[v].deleted && w < dist[u][v]) {
                dist[u][v] = w;
                if (next) next[u][v] = v;
            }
            cur = cur->next;
        }
    }
    for (int k = 0; k < n; k++) {
        if (graph->vertices[k].deleted) continue;
        for (int i = 0; i < n; i++) {
            if (graph->vertices[i].deleted || dist[i][k] == INF) continue;
            for (int j = 0; j < n; j++) {
                if (graph->vertices[j].deleted || dist[k][j] == INF) continue;
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    if (next) next[i][j] = next[i][k];
                }
            }
        }
    }
    int negative = 0;
    for (int i = 0; i < n; i++) {
        if (!graph->vertices[i].deleted && dist[i][i] < 0) {
            negative = 1;
            break;
        }
    }
    *dist_out = dist;
    if (next_out)
        *next_out = next;
    else if (next) {
        for (int i = 0; i < n; i++) free(next[i]);
        free(next);
    }
    return negative ? 1 : 0;
}

void free_floyd(int** matrix, int n) {
    for (int i = 0; i < n; i++) free(matrix[i]);
    free(matrix);
}

void find_max_distance_pair(Graph* graph, int** dist) {
    int n = graph->len;
    int INF = INT_MAX/2;
    int max_dist = -INF;
    int from = -1, to = -1;
    for (int i = 0; i < n; i++) {
        if (graph->vertices[i].deleted) continue;
        for (int j = 0; j < n; j++) {
            if (i == j || graph->vertices[j].deleted) continue;
            if (dist[i][j] != INF && dist[i][j] > max_dist) {
                max_dist = dist[i][j];
                from = i;
                to = j;
            }
        }
    }
    if (from != -1) {
        printf("Пара с максимальным кратчайшим расстоянием:\n");
        printf("%s -> %s, сумма оценок = %d\n",
               graph->vertices[from].name, graph->vertices[to].name, max_dist);
    } else {
        printf("Нет достижимых пар.\n");
    }
}

void print_path_floyd(Graph* graph, int** next, int u, int v) {
    if (next[u][v] == -1) {
        printf("Путь не существует.\n");
        return;
    }
    printf("%s", graph->vertices[u].name);
    while (u != v) {
        u = next[u][v];
        printf(" -> %s", graph->vertices[u].name);
    }
    printf("\n");
}