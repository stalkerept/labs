#ifndef UTILS_H
#define UTILS_H

#include "storage.h"

int read_board_size(void);
char read_move(void);
void display_board(Storage* s, int size, long long score);
void display_game_over(int won, long long score);

#endif