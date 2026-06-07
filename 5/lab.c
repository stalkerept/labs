#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "func.h"
#include "utils.h"

int main(void){
    int capacity = 100;
    printf("Введите начальную ёмкость графа (макс. число вершин): ");
    if (scanf("%d", &capacity) != 1 || capacity <= 0) {
        capacity = 100;
        printf("Установлена ёмкость по умолчанию: 100\n");
    }
    while (getchar() != '\n');
    Graph* graph = init_graph(capacity);
    if (!graph) {
        fprintf(stderr, "Ошибка инициализации графа.\n");
        return 1;
    }
    int choice = 0;
    while (choice != 13) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Некорректный ввод. Завершение работы.\n");
            break;
        }
        while (getchar() != '\n');
        switch (choice) {
            case 1: handle_add_vertex(graph); break;
            case 2: handle_add_edge(graph); break;
            case 3: handle_delete_vertex(graph); break;
            case 4: handle_delete_edge(graph); break;
            case 5: handle_change_vertex_name(graph); break;
            case 6: handle_change_edge_weight(graph); break;
            case 7: handle_print_adjacency(graph); break;
            case 8: handle_graphviz(graph); break;
            case 9: handle_dfs(graph); break;
            case 10: handle_dijkstra(graph); break;
            case 11: handle_special_operation(graph); break;
            case 12: handle_load_from_file(graph); break;
            case 13: printf("Выход.\n"); break;
            default: printf("Неизвестная команда.\n");
        }
    }
    free_graph(graph);
    return 0;
}