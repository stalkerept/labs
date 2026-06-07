#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

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
    printf("0. Выход\n");
    printf("Ваш выбор: ");
}
