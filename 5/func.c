#include <stdlib.h>
#include <string.h>
#include "structures.h"

Graph* init_graph(int capacity){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if(!graph) return NULL;
    graph -> vertices = (Vertex*)malloc(sizeof(Vertex)*capacity);
    if(!graph -> vertices){
        free(graph);
        return NULL;
    }
    graph -> heads = (AdjNode**)malloc(sizeof(AdjNode*)*capacity);
    if(!graph -> heads){
        free(graph -> vertices);
        free(graph);
        return NULL;
    }
    graph -> len = 0;
    graph -> capacity = capacity;
    for(int i = 0; i < graph -> capacity; i++){
        graph -> vertices[i].deleted = 1;
        graph -> vertices[i].name = NULL;
        graph -> vertices[i].color = WHITE;
        graph -> heads[i] = NULL;
    }
    return graph;
}

int find(Graph* graph, Vertex vertex){
    for(int i = 0; i<graph -> len; i++){
        if (graph->vertices[i].deleted) continue;
        if(strcmp(graph -> vertices[i].name, vertex.name) == 0) return i;
    }
    return -1;
}

int get_vertex_index_by_name(const Graph* graph, const char* name) {
    for (int i = 0; i < graph->len; i++){
        if (graph->vertices[i].deleted) continue;
        if(strcmp(graph -> vertices[i].name, name) == 0) return i;
    }
    return -1;
}

Vertex init_vertex(const char* name){
    Vertex new_vertex;
    new_vertex.name = strdup(name);
    if(!new_vertex.name){
        return (Vertex){NULL, WHITE, 0};
    }
    new_vertex.color = WHITE;
    new_vertex.deleted = 0;
    return new_vertex;
}

AdjNode* init_node(int weight, int dest){
    AdjNode* new_node =(AdjNode*)malloc(sizeof(AdjNode));
    if(!new_node) return NULL;
    new_node -> dest = dest, new_node ->weight =weight, new_node -> next = NULL;
    return new_node;
}

int add_vertex(Graph* graph, const char* name){
    if(!graph || !graph -> vertices || !graph -> heads || !name) return -1;
    Vertex v = init_vertex(name);
    if(!v.name) return -1;
    int ind = find(graph, v);
    if(ind != -1){
        free(v.name);
        return ind;
    }
    int free_ind = -1;
    for(int i = 0; i < graph -> capacity; i++){
        if(graph -> vertices[i].deleted != 0){
            free_ind = i;
            break;
        }
    }
    if(free_ind==-1){
        if(graph -> len < graph -> capacity){
            graph -> vertices[graph -> len] = v;
            graph -> heads[graph -> len] = NULL;
            return graph -> len++;
        }
        return -1;
    }
    else{
        free(graph -> vertices[free_ind].name);
        graph -> vertices[free_ind] = v;
        graph -> heads[free_ind] = NULL;
        if(graph -> len <= free_ind) graph -> len = free_ind + 1;
        return free_ind;
    }
}

int add_edge(Graph* graph, int u, int v, int weight){
    if(!graph || !graph ->vertices || !graph ->heads || u<0 || u>=graph -> len || v < 0 || v >= graph -> len) return 1;
    if(graph -> vertices[u].deleted || graph -> vertices[v].deleted) return 1;
    for(AdjNode* cur = graph -> heads[u]; cur; cur = cur -> next){
        if(cur -> dest == v) return 1;
    }
    AdjNode* node_1 = init_node(weight, v);
    if(!node_1) return 1;
    node_1 -> next = graph -> heads[u];
    graph -> heads[u] = node_1;
    AdjNode* node_2 = init_node(weight, u);
    if(!node_2) return 1;
    node_2 -> next = graph -> heads[v];
    graph -> heads[v] = node_2;
    return 0;
}

char delete_edge(Graph* graph, int u, int v){
    if(!graph || !graph -> vertices || !graph -> heads || graph -> len <= 0 || graph -> capacity <=0 || u<0 || u>=graph -> len || v<0 || v >= graph -> len) return 1;
    AdjNode* curr = graph -> heads[u], *prev = NULL;
    while(curr){
        if(curr -> dest == v){
            if(prev) prev -> next = curr -> next;
            else graph -> heads[u] = curr -> next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr -> next;
    }

    AdjNode* curr2 = graph -> heads[v], *prev2 = NULL;
    while(curr2){
        if(curr2 -> dest == u){
            if(prev2) prev2 -> next = curr2 -> next;
            else graph -> heads[v] = curr2 -> next;
            free(curr2);
            break;
        }
        prev2 = curr2;
        curr2 = curr2 -> next;
    }
    return 0;
}


char delete_vertex(Graph* graph, const char* name){
    if(!graph || !graph -> vertices || !graph -> heads || !name || graph -> len <= 0 || graph -> capacity <=0) return 1;
    int ind = get_vertex_index_by_name(graph, name);
    if(ind == -1) return 1;
    while(graph -> heads[ind]){
        int res = delete_edge(graph, graph->heads[ind]->dest, ind);
        if(res != 0) return 1;
    }
    free(graph -> vertices[ind].name);
    graph -> vertices[ind].name = NULL;
    graph -> vertices[ind].deleted = 1;
    return 0;
}

void free_graph(Graph* graph){
    if(!graph || !graph -> vertices || !graph -> heads) return;
    for(int i = 0; i<graph->len; i++){
        free(graph -> vertices[i].name);
    }
    free(graph -> vertices);
    for(int i = 0; i<graph->len; i++){
        AdjNode* now = graph ->heads[i];
        while(now){
            AdjNode* prev = now;
            now = now -> next;
            free(prev);
        }
    }
    free(graph -> heads);
    free(graph);
    return;
}