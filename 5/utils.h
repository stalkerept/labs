#ifndef UTILS_H
#define UTILS_H
#include "structures.h"

void show_menu();
void handle_add_vertex(Graph* graph);
void handle_add_edge(Graph* graph);
void handle_delete_vertex(Graph* graph);
void handle_delete_edge(Graph* graph);
void handle_change_vertex_name(Graph* graph);
void handle_change_edge_weight(Graph* graph);
void handle_print_adjacency(const Graph* graph);
void handle_graphviz(const Graph* graph);
void handle_dfs(Graph* graph);
void handle_dijkstra(Graph* graph);
void handle_special_operation(Graph* graph);
void handle_load_from_file(Graph* graph);
char* read_line(const char* prompt);
int read_int(const char* prompt, int min, int max);
int load_from_file(Graph* graph, const char* filename);
#endif