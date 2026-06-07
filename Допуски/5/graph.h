#ifndef GRAPH_H
#define GRAPH_H

typedef struct Edge
{
    // Номера вершин соответствуют их позиции в векторе вершин
    int from; // Номер вершины, из которой выходит ребро
    int to; // Номер вершины, в которую входит ребро
    int weight; // Вес ребра
} Edge;

typedef struct Vertex
{
    int edge_count; // Количество исходящих ребер
    Edge* edges; // Вектор исходящих ребер
} Vertex;

typedef struct Graph
{
    int vertex_count; // Количество вершин
    Vertex* vertices; // Вектор вершин
} Graph;

#endif