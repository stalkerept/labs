#include "game.h"
#include "utils.h"
#include <stdio.h>

int main(void) {
    int size = read_board_size();
    if (size == 0) return 0;
    run_game(size);
    return 0;
}