#ifndef UTILS_H
#define UTILS_H

int read_board_size(void);
char read_move(void);
void display_board(int** grid, int size, long long score);
void display_game_over(int won, long long score);

#endif