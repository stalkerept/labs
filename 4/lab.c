#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structures.h"
#include "func.h"
#include "utils.h"

#define MAX_CMD_LEN 256

void print_help() {
    printf("Доступные команды:\n");
    printf("  add <ключ> <значение>   - добавить запись (при существующем ключе обновить значение)\n");
    printf("  delete <ключ>           - удалить запись с указанным ключом\n");
    printf("  find <ключ>             - найти запись по ключу\n");
    printf("  special <ключ>          - найти наименьший ключ, превышающий заданный\n");
    printf("  print [мин] [макс]      - обход в обратном порядке (RNL) в диапазоне ключей\n");
    printf("  format                  - вывести дерево в консоль в виде дерева\n");
    printf("  graph <имя_файла>       - сохранить изображение дерева через Graphviz\n");
    printf("  load <имя_файла>        - загрузить пары ключ-значение из текстового файла\n");
    printf("  help                    - показать это сообщение\n");
    printf("  quit / exit             - выйти из программы\n");
}

int main() {
    Tree tree = {NULL, 0};
    char input[MAX_CMD_LEN];
    char command[MAX_CMD_LEN];
    printf("Программа работы с двоичным деревом поиска. Введите 'help' для списка команд.\n");
    while (1) {
        printf("\n> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) continue;
        char input_copy[MAX_CMD_LEN];
        strncpy(input_copy, input, MAX_CMD_LEN);
        input_copy[MAX_CMD_LEN-1] = '\0';
        char* token = strtok(input_copy, " \t");
        if (!token) continue;
        strncpy(command, token, MAX_CMD_LEN);
        command[MAX_CMD_LEN-1] = '\0';

        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "help") == 0) {
            print_help();
        } else if (strcmp(command, "add") == 0) {
            char *key = NULL, *value = NULL;
            if (parse_add_input(input, &key, &value)) {
                char* old = insert(&tree, key, value);
                if (old) {
                    printf("Обновлено значение для ключа '%s'. Старое значение: '%s'\n", key, old);
                    free(old);
                } else {
                    printf("Добавлена запись: '%s' -> '%s'\n", key, value);
                }
                free(key);
                free(value);
            } else {
                printf("Ошибка: неверный формат команды. Используйте: add <ключ> <значение>\n");
            }
        } else if (strcmp(command, "delete") == 0) {
            token = strtok(NULL, " \t");
            if (!token) {
                printf("Ошибка: укажите ключ для удаления.\n");
                continue;
            }
            char* key = token;
            if (delete(&tree, key) == 0) {
                printf("Запись с ключом '%s' удалена.\n", key);
            } else {
                printf("Запись с ключом '%s' не найдена.\n", key);
            }
        } else if (strcmp(command, "find") == 0) {
            token = strtok(NULL, " \t");
            if (!token) {
                printf("Ошибка: укажите ключ для поиска.\n");
                continue;
            }
            Node* node = find(&tree, token);
            if (node) {
                printf("Найдено: ключ='%s', значение='%s'\n", node->key, node->info);
            } else {
                printf("Запись с ключом '%s' не найдена.\n", token);
            }
        } else if (strcmp(command, "special") == 0) {
            token = strtok(NULL, " \t");
            if (!token) {
                printf("Ошибка: укажите ключ.\n");
                continue;
            }
            Node* node = find_special(&tree, token);
            if (node) {
                printf("Наименьший ключ > '%s': '%s' -> '%s'\n", token, node->key, node->info);
            } else {
                printf("Нет ключа, превышающего '%s'.\n", token);
            }
        } else if (strcmp(command, "print") == 0) {
            char* min_key = strtok(NULL, " \t");
            char* max_key = strtok(NULL, " \t");
            if (!min_key) min_key = "";
            if (!max_key) max_key = min_key;
            if (strlen(min_key) == 0) {
                min_key = "\x01";  // минимально возможная строка (больше \0)
                max_key = "\xFF";  // максимально возможная
            }
            printf("Обход RNL в диапазоне [%s, %s]:\n", min_key, max_key);
            print_RNL(&tree, min_key, max_key);
            printf("\n");
        } else if (strcmp(command, "format") == 0) {
            print_tree_visual(tree.root, 10);
        } else if (strcmp(command, "graph") == 0) {
            token = strtok(NULL, " \t");
            if (!token) {
                printf("Ошибка: укажите имя файла (без расширения).\n");
                continue;
            }
            if (generate_graphviz(&tree, token) != 0) {
                printf("Не удалось создать изображение.\n");
            }
        } else if (strcmp(command, "load") == 0) {
            token = strtok(NULL, " \t");
            if (!token) {
                printf("Ошибка: укажите имя файла.\n");
                continue;
            }
            int count = load_tree_from_file(&tree, token);
            if (count >= 0) {
                printf("Загружено %d записей из файла '%s'.\n", count, token);
            } else {
                printf("Ошибка при загрузке файла '%s'.\n", token);
            }
        } else {
            printf("Неизвестная команда. Введите 'help' для списка.\n");
        }
    }
    free_tree(&tree);
    printf("Программа завершена.\n");
    return 0;
}