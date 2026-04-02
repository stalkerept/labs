#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "func.h"
#include "utils.h"


int main() {
    Table table;
    int table_initialized = 0;
    int choice;
    unsigned int key;
    char info[1024];
    int release;
    int mode; // для удаления (1 – все версии, 0 – только первый)
    Table *result = NULL;

    while (1) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода. Завершение.\n");
            if (table_initialized) {
                free_table(&table); 
            }
            break;
        }
        clear_input();
        switch (choice) {
            case 0:
                if (table_initialized) {
                    free_table(&table);
                }
                printf("Выход.\n");
                return 0;

            case 1:
                if (table_initialized) {
                    printf("Таблица уже создана. Сначала очистите её.\n");
                    break;
                }
                {
                    int msize;
                    printf("Введите максимальное количество различных ключей: ");
                    if (scanf("%d", &msize) != 1 || msize <= 0) {
                        printf("Некорректный размер.\n");
                        clear_input();
                        break;
                    }
                    clear_input();
                    if (init(&table, msize) != 0) {
                        printf("Ошибка инициализации таблицы.\n");
                    } else {
                        table_initialized = 1;
                        printf("Таблица создана (максимум %d ключей).\n", msize);
                    }
                }
                break;

            case 2:
                if (!table_initialized) {
                    printf("Таблица не создана.\n");
                    break;
                }
                printf("Введите ключ (целое неотрицательное): ");
                if (read_unsigned_int(&key) != 0) {
                    printf("Ошибка ввода ключа.\n");
                    break;
                }
                printf("Введите строковое значение: ");
                if (read_string(info, sizeof(info)) != 0) {
                    printf("Ошибка ввода значения.\n");
                    break;
                }
                if (insert(&table, key, info) != 0) {
                    printf("Ошибка вставки (возможно, таблица полна).\n");
                } else {
                    printf("Элемент вставлен.\n");
                }
                break;

            case 3:
                if (!table_initialized) {
                    printf("Таблица не создана.\n");
                    break;
                }
                printf("Введите ключ для удаления: ");
                if (read_unsigned_int(&key) != 0) {
                    printf("Ошибка ввода ключа.\n");
                    break;
                }
                printf("Выберите режим удаления:\n");
                printf("1 - удалить все версии этого ключа\n");
                printf("2 - удалить конкретную версию (укажите номер)\n");
                printf("3 - удалить только первый элемент (без учёта версии)\n");
                printf("Ваш выбор: ");
                if (scanf("%d", &mode) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input();
                    break;
                }
                clear_input();
                if (mode == 1) {
                    if (erase(&table, key, 0, 1) != 0) {
                        printf("Ключ не найден или ошибка удаления.\n");
                    } else {
                        printf("Все версии ключа %u удалены.\n", key);
                    }
                } else if (mode == 2) {
                    printf("Введите номер версии: ");
                    if (scanf("%d", &release) != 1 || release <= 0) {
                        printf("Некорректный номер версии.\n");
                        clear_input();
                        break;
                    }
                    clear_input();
                    if (erase(&table, key, release, 0) != 0) {
                        printf("Версия %d для ключа %u не найдена.\n", release, key);
                    } else {
                        printf("Версия %d для ключа %u удалена.\n", release, key);
                    }
                } else if (mode == 3) {
                    // удалить только первый элемент (release=0, mode=0)
                    if (erase(&table, key, 0, 0) != 0) {
                        printf("Ключ не найден или ошибка удаления.\n");
                    } else {
                        printf("Первый элемент (старейшая версия) ключа %u удалён.\n", key);
                    }
                } else {
                    printf("Неверный режим.\n");
                }
                break;

            case 4:
                if (!table_initialized) {
                    printf("Таблица не создана.\n");
                    break;
                }
                printf("Введите ключ для поиска: ");
                if (read_unsigned_int(&key) != 0) {
                    printf("Ошибка ввода ключа.\n");
                    break;
                }
                printf("Выберите режим поиска:\n");
                printf("1 - найти все версии ключа\n");
                printf("2 - найти конкретную версию\n");
                printf("Ваш выбор: ");
                if (scanf("%d", &mode) != 1) {
                    printf("Ошибка ввода.\n");
                    clear_input();
                    break;
                }
                clear_input();
                if (mode == 1) {
                    result = find_individual(&table, key, 0);
                } else if (mode == 2) {
                    printf("Введите номер версии: ");
                    if (scanf("%d", &release) != 1 || release <= 0) {
                        printf("Некорректный номер версии.\n");
                        clear_input();
                        break;
                    }
                    clear_input();
                    result = find_individual(&table, key, release);
                } else {
                    printf("Неверный режим.\n");
                    break;
                }
                if (result == NULL) {
                    printf("Ошибка при поиске (возможно, не удалось создать таблицу результатов).\n");
                } else if (result->size == 0) {
                    printf("Элементы не найдены.\n");
                    free_table(result);
                    free(result);
                } else {
                    printf("Найденные элементы:\n");
                    print_table(result);
                    free_table(result);
                    free(result);
                }
                break;

            case 5:
                if (!table_initialized) {
                    printf("Таблица не создана.\n");
                    break;
                }
                printf("Содержимое таблицы:\n");
                print_table(&table);
                break;

            case 6:
                if (!table_initialized) {
                    printf("Таблица не создана.\n");
                    break;
                }
                free_table(&table);
                // повторно инициализируем пустую таблицу того же размера
                if (init(&table, table.msize) != 0) {
                    printf("Ошибка при очистке таблицы.\n");
                    table_initialized = 0;
                } else {
                    printf("Таблица очищена.\n");
                }
                break;
            case 7:
                if (!table_initialized) {
                    printf("Таблица не создана.\n");
                    break;
                }
                printf("Введите имя файла для импорта: ");
                char filename[256];
                if (read_string(filename, sizeof(filename)) != 0) {
                    printf("Ошибка ввода имени файла.\n");
                    break;
                }
                int inserted = import_from_file(&table, filename);
                if (inserted < 0) {
                    printf("Ошибка при открытии или чтении файла '%s'.\n", filename);
                } else {
                    printf("Импортировано %d записей.\n", inserted);
                }
                break;
            default:
                printf("Неверный пункт меню.\n");
                break;
        }
    }
    return 0;
}