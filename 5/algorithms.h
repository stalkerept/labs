#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "structures.h"

void reset_colors(Graph* graph);
void dfs(Graph* graph, int start);
void dfs_full(Graph* graph);
int dijkstra(Graph* graph, int start, int* dist, int* prev);
void print_path(Graph* graph, int* prev, int target);
int floyd_warshall(Graph* graph, int*** dist_out, int*** next_out);
void free_floyd(int** matrix, int n);
void print_path_floyd(Graph* graph, int** next, int u, int v);
void find_max_distance_pair(Graph* graph, int** dist);

#endif