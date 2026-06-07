#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "func.h"
#include <ctype.h>


void print_tree_visual(Node* node, int space) {
    if (!node) return;
    space += 10;
    print_tree_visual(node->right, space);
    printf("\n");
    for (int i = 10; i < space; i++) printf(" ");
    printf("%s\n", node->info);
    print_tree_visual(node->left, space);
}

void print_RNL(Tree* tree, const char* min_key, const char* max_key) {
    if (!tree || !tree->root || tree -> count <= 0) return;
    Stack* stack = stack_init(tree -> count);
    if (!stack) return;
    Node* curr = tree->root;
    while (curr || stack -> top > 0) {
        if (curr) {
            stack -> array[stack -> top++] = curr;
            curr = curr->right;
        } else {
            curr = stack -> array[--stack -> top];
            if (strcmp(curr->key, min_key) >= 0 && strcmp(curr->key, max_key) <= 0) {
                printf("[%s - %s] ", curr->key, curr->info);
            }
            curr = curr->left;
        }
    }
    free(stack -> array);
    free(stack);
}

static void generate_dot_node(FILE* file, const Node* node){
    if(!node) return;
    fprintf(file, "  \"%s\" [label=\"%s\\n%s\"];\n", node -> key, node -> key, node -> info);
    if(node -> left){
        fprintf(file, "  \"%s\" -> \"%s\" [label=\"L\"];\n", node -> key, node ->left->key);
        generate_dot_node(file, node -> left);
    }
    if (node->right) {
        fprintf(file, "  \"%s\" -> \"%s\" [label=\"R\"];\n", node->key, node->right->key);
        generate_dot_node(file, node->right);
    }
    return;
}

int generate_graphviz(Tree* tree, const char* filename){
    if(!tree || !filename) return 1;
    char* dot_filename = (char*)malloc(sizeof(char)*256);
    snprintf(dot_filename, sizeof(dot_filename), "%s.dot", filename);
    FILE* file = fopen(dot_filename, "w");
    if(!file){
        perror("Не получается создать dot файл");
        return 1;
    }
    fprintf(file, "digraph BST {\n");
    fprintf(file, "  node [shape=record];\n");
    if(tree -> root) generate_dot_node(file, tree->root);
    fprintf(file, "}\n");
    fclose(file);
    char command[512];
    snprintf(command, sizeof(command), "dot -Tpng %s.dot -o %s.png", filename, filename);
    int ret = system(command);
    if (ret != 0) {
        fprintf(stderr, "Ошибка вызова Graphviz. Убедитесь, что 'dot' установлен.\n");
        return 1;
    }
    printf("Изображение было сохранено как '%s.png'\n", filename);
    return 0;
}

// Delete space from start and end.
char* enough_space(char* str){
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    char* end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1]='\0';
    return str;
}


int parse_add_input(char* input, char** key, char** value) {
    char* token = strtok(input, " \t");
    if (!token) return 0;
    token = strtok(NULL, " \t");
    if (!token) return 0;
    *key = strdup(token);
    token = strtok(NULL, "");
    if (!token) {
        free(*key);
        return 0;
    }
    *value = strdup(enough_space(token));
    if (**value == '\0') {
        free(*key);
        free(*value);
        return 0;
    }
    return 1;
}

int load_tree_from_file(Tree* tree, const char* filename) {
    if (!tree || !filename) return -1;
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Ошибка открытия файла");
        return -1;
    }
    char key[256], value[256];
    int loaded = 0;
    while (fscanf(fp, "%255s %255s", key, value) == 2) {
        char* old = insert(tree, key, value);
        if (old) {
            free(old);
        }
        loaded++;
    }
    fclose(fp);
    return loaded;
}