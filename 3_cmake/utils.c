#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "structures.h"
#include "func.h"
#include <ctype.h> 


int read_unsigned_int(unsigned int *value) {
    char buffer[32];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return -1; // EOF
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        return -1; // пустой ввод
    }
    char *endptr;
    errno = 0;
    unsigned long val = strtoul(buffer, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || val > UINT_MAX) {
        return -1; // некорректное число
    }
    *value = (unsigned int)val;
    return 0;
}

int read_string(char *buffer, size_t max_len) {
    if (!fgets(buffer, max_len, stdin)) {
        return -1; // EOF
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == max_len - 1 && buffer[max_len - 2] != '\n') {
        clear_input();
    }
    return 0;
}

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void show_menu() {
    printf("\n=== Меню ===\n");
    printf("1. Создать таблицу (указать размер)\n");
    printf("2. Вставить элемент\n");
    printf("3. Удалить элемент(ы)\n");
    printf("4. Найти элемент(ы)\n");
    printf("5. Вывести таблицу\n");
    printf("6. Очистить таблицу\n");
    printf("7. Импортировать данные из файла\n");
    printf("0. Выход\n");
    printf("Ваш выбор: ");
}

// Вспомогательная функция: удаляет пробельные символы в начале и конце строки
static void trim(char *str) {
    if (!str) return;
    char *start = str;
    char *end;
    // удаляем пробелы в начале
    while (isspace((unsigned char)*start)) start++;
    // если строка состояла из одних пробелов
    if (*start == '\0') {
        str[0] = '\0';
        return;
    }
    // удаляем пробелы в конце
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    // сдвигаем строку в начало буфера
    memmove(str, start, end - start + 2);
}

int import_from_file(Table *table, const char *filename) {
    if (!table || !filename) return -1;
    FILE *file = fopen(filename, "r");
    if (!file) return -1;
    char line[1024 + 32];
    int count = 0;
    unsigned int key;
    char info[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        // обрезаем пробелы в начале и конце
        trim(line);
        // пропускаем пустые строки
        if (strlen(line) == 0) continue;
        char *space = strchr(line, ' ');
        if (!space) {
            fprintf(stderr, "Пропущена строка без пробела: '%s'\n", line);
            continue;
        }
        *space = '\0';
        char *key_str = line;
        char *info_str = space + 1;
        trim(info_str);
        if (strlen(info_str) == 0) {
            fprintf(stderr, "Пропущена строка с пустым значением info: ключ '%s'\n", key_str);
            continue;
        }
        char *endptr;
        unsigned long val = strtoul(key_str, &endptr, 10);
        if (*endptr != '\0' || val > UINT_MAX) {
            fprintf(stderr, "Некорректный ключ в строке: '%s'\n", key_str);
            continue;
        }
        key = (unsigned int)val;
        strncpy(info, info_str, sizeof(info) - 1);
        info[sizeof(info) - 1] = '\0';
        if (insert(table, key, info) == 0) {
            count++;
        } else {
            fprintf(stderr, "Ошибка вставки: ключ %u, info '%s'\n", key, info);
        }
    }
    fclose(file);
    return count;
}