#include "game.h"
#include "utils.h"
#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define PROB_4  10
#define KEY_BUF_SIZE 32

static int get_cell(Storage* s, int x, int y) {
    char buf[KEY_BUF_SIZE];
    snprintf(buf, sizeof(buf), "%d,%d", x, y);
    int val = storage_find(s, buf);
    return (val == -1) ? 0 : val;
}

static void set_cell(Storage* s, int x, int y, int val) {
    char buf[KEY_BUF_SIZE];
    snprintf(buf, sizeof(buf), "%d,%d", x, y);
    if (val == 0) {
        storage_delete(s, buf);
    } else {
        storage_insert(s, buf, val);
    }
}

static int has_empty_cell(Storage* s, int size) {
    int res = storage_have_empty_cell(s, size);
    return (res == 1);
}

static int has_won(Storage* s) {
    return storage_max_tile(s) >= 2048;
}



static int can_move(Storage* s, int size) {
    if (has_empty_cell(s, size))
        return 1;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int val = get_cell(s, x, y);
            if (val == 0) continue;
            if (x + 1 < size) {
                int right = get_cell(s, x + 1, y);
                if (right == val) return 1;
            }
            if (y + 1 < size) {
                int down = get_cell(s, x, y + 1);
                if (down == val) return 1;
            }
        }
    }
    return 0;
}

static long long shift_line(int* line, int length) {
    long long points = 0;
    int write = 0;
    for (int read = 0; read < length; read++) {
        if (line[read] != 0) {
            if (write > 0 && line[write - 1] == line[read]) {
                line[write - 1] *= 2;
                points += line[write - 1];
            } else {
                line[write++] = line[read];
            }
        }
    }
    while (write < length) line[write++] = 0;
    return points;
}

static int add_random_tile(Storage* s, int size, long long* score) {
    (void)score;
    int empty_count = 0;
    int* empty_x = malloc(size * size * sizeof(int));
    int* empty_y = malloc(size * size * sizeof(int));
    if (!empty_x || !empty_y) {
        free(empty_x);
        free(empty_y);
        return 1;
    }
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (get_cell(s, x, y) == 0) {
                empty_x[empty_count] = x;
                empty_y[empty_count] = y;
                empty_count++;
            }
        }
    }
    if (empty_count == 0) {
        free(empty_x);
        free(empty_y);
        return 1;
    }
    int idx = rand() % empty_count;
    int x = empty_x[idx];
    int y = empty_y[idx];
    free(empty_x);
    free(empty_y);
    int new_val = (rand() % 100 < PROB_4) ? 4 : 2;
    set_cell(s, x, y, new_val);
    return 0;
}

static int move(Storage* s, int size, char dir, long long* score) {
    int moved = 0;
    long long gain = 0;
    if (dir == 'a') { // влево
        for (int y = 0; y < size; y++) {
            int* line = malloc(size * sizeof(int));
            if (!line) continue;
            for (int x = 0; x < size; x++)
                line[x] = get_cell(s, x, y);
            gain += shift_line(line, size);
            for (int x = 0; x < size; x++) {
                int old = get_cell(s, x, y);
                if (old != line[x]) {
                    set_cell(s, x, y, line[x]);
                    moved = 1;
                }
            }
            free(line);
        }
    }
    else if (dir == 'd') { // вправо
        for (int y = 0; y < size; y++) {
            int* line = malloc(size * sizeof(int));
            if (!line) continue;
            for (int x = 0; x < size; x++)
                line[size - 1 - x] = get_cell(s, x, y);
            gain += shift_line(line, size);
            for (int x = 0; x < size; x++) {
                int new_val = line[size - 1 - x];
                int old = get_cell(s, x, y);
                if (old != new_val) {
                    set_cell(s, x, y, new_val);
                    moved = 1;
                }
            }
            free(line);
        }
    }
    else if (dir == 'w') { // вверх
        for (int x = 0; x < size; x++) {
            int* col = malloc(size * sizeof(int));
            if (!col) continue;
            for (int y = 0; y < size; y++)
                col[y] = get_cell(s, x, y);
            gain += shift_line(col, size);
            for (int y = 0; y < size; y++) {
                int old = get_cell(s, x, y);
                if (old != col[y]) {
                    set_cell(s, x, y, col[y]);
                    moved = 1;
                }
            }
            free(col);
        }
    }
    else if (dir == 's') { // вниз
        for (int x = 0; x < size; x++) {
            int* col = malloc(size * sizeof(int));
            if (!col) continue;
            for (int y = 0; y < size; y++)
                col[size - 1 - y] = get_cell(s, x, y);
            gain += shift_line(col, size);
            for (int y = 0; y < size; y++) {
                int new_val = col[size - 1 - y];
                int old = get_cell(s, x, y);
                if (old != new_val) {
                    set_cell(s, x, y, new_val);
                    moved = 1;
                }
            }
            free(col);
        }
    }
    if (moved) {
        *score += gain;
        add_random_tile(s, size, score);
    }
    return moved;
}

void run_game(int size) {
    srand(time(NULL));
    Storage* storage = storage_init(size * size * 2);
    if (!storage) {
        fprintf(stderr, "Ошибка инициализации хранилища\n");
        return;
    }
    long long score = 0;
    add_random_tile(storage, size, &score);
    add_random_tile(storage, size, &score);
    int game_over = 0;
    int won = 0;
    int quit = 0;
    while (!game_over && !quit) {
        display_board(storage, size, score);
        char dir = read_move();
        if (dir == 'q') {
            quit = 1;
            break;
        }
        if (move(storage, size, dir, &score)) {
            if (has_won(storage)) {
                won = 1;
                game_over = 1;
            } else if (!can_move(storage, size)) {
                game_over = 1;
            }
        } else {
            printf("Невозможный ход. Попробуйте ещё раз.\n");
        }
    }

    display_board(storage, size, score);
    display_game_over(won, score);
    storage_free(storage);
}