#include "game.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PROB_4  10
#define KEY_BUF_SIZE 32

static void make_key(int x, int y, char* buf, int bufsize) {
    snprintf(buf, bufsize, "%d,%d", x, y);
}

static int has_empty_cell(Storage* s, int size) {
    char buf[KEY_BUF_SIZE];
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++) {
            make_key(c, r, buf, sizeof(buf));
            if (storage_find(s, buf) == -1)
                return 1;
        }
    return 0;
}

static int can_move(Storage* s, int size) {
    if (has_empty_cell(s, size)) return 1;
    char buf1[KEY_BUF_SIZE], buf2[KEY_BUF_SIZE];
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            make_key(c, r, buf1, sizeof(buf1));
            int val = storage_find(s, buf1);
            if (val == -1) continue;
            if (c + 1 < size) {
                make_key(c+1, r, buf2, sizeof(buf2));
                int val2 = storage_find(s, buf2);
                if (val2 != -1 && val == val2) return 1;
            }
            if (r + 1 < size) {
                make_key(c, r+1, buf2, sizeof(buf2));
                int val2 = storage_find(s, buf2);
                if (val2 != -1 && val == val2) return 1;
            }
        }
    }
    return 0;
}

static int add_random_tile(Storage* s, int size, long long* score) {
    (void)score;
    if (!has_empty_cell(s, size)) return 1;
    int x, y;
    char buf[KEY_BUF_SIZE];
    do {
        x = rand() % size;
        y = rand() % size;
        make_key(x, y, buf, sizeof(buf));
    } while (storage_find(s, buf) != -1);
    int new_val = (rand() % 100 < PROB_4) ? 4 : 2;
    storage_insert(s, buf, new_val);
    return 0;
}

static void storage_to_grid(Storage* s, int size, int** grid) {
    char buf[KEY_BUF_SIZE];
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++) {
            make_key(c, r, buf, sizeof(buf));
            int val = storage_find(s, buf);
            grid[r][c] = (val == -1) ? 0 : val;
        }
}

static void grid_to_storage(Storage* s, int size, int** grid) {
    char buf[KEY_BUF_SIZE];
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++) {
            make_key(c, r, buf, sizeof(buf));
            storage_delete(s, buf);
        }
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++) {
            if (grid[r][c] != 0) {
                make_key(c, r, buf, sizeof(buf));
                storage_insert(s, buf, grid[r][c]);
            }
        }
}

static long long shift_line(int* line, int length) {
    long long points = 0;
    int pos = 0;
    for (int i = 0; i < length; i++) {
        if (line[i] != 0) {
            if (pos != i) {
                line[pos] = line[i];
                line[i] = 0;
            }
            pos++;
        }
    }
    for (int i = 0; i < length - 1; i++) {
        if (line[i] != 0 && line[i] == line[i+1]) {
            line[i] *= 2;
            points += line[i];
            line[i+1] = 0;
            i++;
        }
    }
    pos = 0;
    for (int i = 0; i < length; i++) {
        if (line[i] != 0) {
            if (pos != i) {
                line[pos] = line[i];
                line[i] = 0;
            }
            pos++;
        }
    }
    return points;
}

static int move(Storage* s, int size, char dir, long long* score) {
    int** grid = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++)
        grid[i] = malloc(size * sizeof(int));
    storage_to_grid(s, size, grid);
    long long gain = 0;
    int moved = 0;
    if (dir == 'a') {
        for (int r = 0; r < size; r++) {
            int line_before[size];
            for (int c = 0; c < size; c++) line_before[c] = grid[r][c];
            gain += shift_line(grid[r], size);
            for (int c = 0; c < size; c++)
                if (grid[r][c] != line_before[c]) moved = 1;
        }
    } else if (dir == 'd') {
        for (int r = 0; r < size; r++) {
            int reversed[size];
            for (int c = 0; c < size; c++) reversed[size-1-c] = grid[r][c];
            int line_before[size];
            memcpy(line_before, grid[r], size*sizeof(int));
            gain += shift_line(reversed, size);
            for (int c = 0; c < size; c++) {
                int new_val = reversed[size-1-c];
                if (grid[r][c] != new_val) moved = 1;
                grid[r][c] = new_val;
            }
        }
    } else if (dir == 'w') {
        for (int c = 0; c < size; c++) {
            int col[size];
            for (int r = 0; r < size; r++) col[r] = grid[r][c];
            int col_before[size];
            memcpy(col_before, col, size*sizeof(int));
            gain += shift_line(col, size);
            for (int r = 0; r < size; r++) {
                if (grid[r][c] != col[r]) moved = 1;
                grid[r][c] = col[r];
            }
        }
    } else if (dir == 's') {
        for (int c = 0; c < size; c++) {
            int col[size];
            for (int r = 0; r < size; r++) col[size-1-r] = grid[r][c];
            int col_before[size];
            memcpy(col_before, col, size*sizeof(int));
            gain += shift_line(col, size);
            for (int r = 0; r < size; r++) {
                int new_val = col[size-1-r];
                if (grid[r][c] != new_val) moved = 1;
                grid[r][c] = new_val;
            }
        }
    }
    if (moved) {
        grid_to_storage(s, size, grid);
        *score += gain;
        add_random_tile(s, size, score);
    }
    for (int i = 0; i < size; i++) free(grid[i]);
    free(grid);
    return moved;
}

static int has_won(Storage* s, int size) {
    char buf[KEY_BUF_SIZE];
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++) {
            make_key(c, r, buf, sizeof(buf));
            int val = storage_find(s, buf);
            if (val == 2048) return 1;
        }
    return 0;
}

void run_game(int size) {
    srand(time(NULL));
    Storage* storage = storage_init(size * size * 2);
    int** display_grid = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++)
        display_grid[i] = malloc(size * sizeof(int));
    long long score = 0;
    add_random_tile(storage, size, &score);
    add_random_tile(storage, size, &score);
    int game_over = 0;
    int won = 0;
    int quit = 0;
    while (!game_over && !quit) {
        storage_to_grid(storage, size, display_grid);
        display_board(display_grid, size, score);
        char dir = read_move();
        if (dir == 'q') {
            quit = 1;
            break;
        }
        if (move(storage, size, dir, &score)) {
            if (has_won(storage, size)) {
                won = 1;
                game_over = 1;
            } else if (!can_move(storage, size)) {
                game_over = 1;
            }
        } else {
            printf("Невозможный ход. Попробуйте ещё раз.\n");
        }
    }
    if (game_over) {
        storage_to_grid(storage, size, display_grid);
        display_board(display_grid, size, score);
        display_game_over(won, score);
    }
    for (int i = 0; i < size; i++)
        free(display_grid[i]);
    free(display_grid);
    storage_free(storage);
}