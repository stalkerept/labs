#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"
#include "list.h"
#include <limits.h>

int shortest_path(const Graph* graph, int from, int to, List* result){
    if(!graph || !result || to < 0 || from < 0 || to >= graph -> vertex_count || from >= graph ->vertex_count) return 1;
    int V = graph -> vertex_count;
    long long* dist = (long long*)malloc(sizeof(long long) * V);
    int* pred = (int*)malloc(sizeof(int)*V);
    bool* affected = (bool*)malloc(sizeof(bool)*V);
    if(!dist || !pred || !affected){
        free(dist);
        free(pred);
        free(affected);
        return 1;
    }
    const long long INF = LLONG_MAX/2;
    for(int i = 0; i < V; i++){
        dist[i] = INF;
        pred[i] = -1;
        affected[i] = false;
    }
    dist[from] = 0;
    // сам алгоритм
    for(int i = 0; i < V-1; i++){
        bool updated = false;
        for(int j = 0; j < V; j++){
            if(dist[j] == INF) continue;
            Vertex* vertex = &graph -> vertices[j];
            for(int u = 0; u < vertex -> edge_count; u++){
                Edge e = vertex -> edges[u];
                int v = e.to;
                long long w = e.weight;
                if(dist[j] + w < dist[v]){
                    dist[v] = dist[j] + w;
                    pred[v] = j;
                    updated = true;
                }
            }
        }
        if(!updated) break;
    }
    for(int u = 0; u < V; u++){
        if(dist[u] == INF) continue;
        Vertex* vertex = &graph -> vertices[u];
        for(int j = 0; j < vertex -> edge_count; j++){
            Edge e = vertex -> edges[j];
            if(dist[u] + e.weight < dist[e.to]) affected[e.to] = true;
        }
    }

    bool changed = true;
    while(changed){
        changed = false;
        for(int u = 0; u<V; u++){
            if(!affected[u]) continue;
            Vertex* vertex = &graph ->vertices[u];
            for(int j = 0; j < vertex -> edge_count; j++){
                int v = vertex -> edges[j].to;
                if(!affected[u]){
                    affected[v] = true;
                    changed = true;
                }
            }
        }
    }
    

}