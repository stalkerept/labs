#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN_BOARD 2
#define MAX_BOARD 100

int read_board_size(void) {
    int n;
    printf("Введите размер поля (N >= 2, максимум %d): ", MAX_BOARD);
    while (1) {
        int ret = scanf("%d", &n);
        if (ret == EOF) {
            printf("\nВыход.\n");
            return 0;
        }
        if (ret != 1 || n < MIN_BOARD || n > MAX_BOARD) {
            printf("Некорректный ввод. Введите число от %d до %d: ", MIN_BOARD, MAX_BOARD);
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return n;
    }
}

char read_move(void) {
    char dir;
    printf("Ход (w-вверх, s-вниз, a-влево, d-вправо, q-выход): ");
    while (1) {
        int ret = scanf(" %c", &dir);
        if (ret == EOF) {
            printf("\nВыход.\n");
            return 'q';
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        if (dir == 'w' || dir == 'a' || dir == 's' || dir == 'd' || dir == 'q')
            return dir;
        printf("Неверное направление. Используйте w/a/s/d или q для выхода: ");
    }
}

void display_board(int** grid, int size, long long score) {
    printf("\nScore: %lld\n", score);
    printf("+");
    for (int i = 0; i < size; i++) printf("----+");
    printf("\n");
    for (int r = 0; r < size; r++) {
        printf("|");
        for (int c = 0; c < size; c++) {
            int val = grid[r][c];
            if (val == 0)
                printf("    |");
            else
                printf("%4d|", val);
        }
        printf("\n+");
        for (int i = 0; i < size; i++) printf("----+");
        printf("\n");
    }
}

void display_game_over(int won, long long score) {
    if (won)
        printf("Поздравляем! Вы достигли 2048!\n");
    else
        printf("Игра окончена. Нет доступных ходов.\n");
    printf("Финальный счёт: %lld\n", score);
}