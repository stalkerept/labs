#ifndef FUNC_H
#define FUNC_H
#include "structures.h"

Graph* init_graph(int capacity);
int find(Graph* graph, Vertex vertex);
Vertex init_vertex(const char* name);
AdjNode* init_node(int weight, int dest);
int add_vertex(Graph* graph, const char* name);
int add_edge(Graph* graph, int u, int v, int weight);
char delete_edge(Graph* graph, int u, int v);
char delete_vertex(Graph* graph, const char* name);
void free_graph(Graph* graph);
int get_vertex_index_by_name(const Graph* graph, const char* name);
#endif