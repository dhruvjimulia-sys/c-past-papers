#include "laser.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_on_board(board_t board, int row, int col) {
    return true;
}

int find_start(board_t board) {
    return -1;
}

char get_mirror_label(board_t board, int row, int col) {
    assert(is_on_board(board, row, col));
    return '\0';
}

board_t load_board(const char *filename) {
    board_t board;
    return board;
}

void free_cells(board_t board) {
}

direction_t change_direction(direction_t direction, char cell) {
    return direction;
}

char step_laser(board_t board, laser_state_t *laser) {
    assert(is_on_board(board, laser->row, laser->col));
    return '\0';
}

bool shoot(board_t board, laser_state_t *laser, bool trace) {
    assert(is_on_board(board, laser->row, laser->col));
    return false;
}

bool solve(board_t board, laser_state_t laser, const char target[MAX_MSG_LEN]) {
    assert(is_on_board(board, laser.row, laser.col));
    return false;
}

#ifdef MAIN
int main(void) {
    board_t board = load_board("biscuit.txt");
    assert(find_start(board) != -1);
    printf("Initial board:\n");
    print_board(board);

    const char target[MAX_MSG_LEN] = "BIUC";
    printf("Attempt to construct word: %s\n", target);

    laser_state_t laser = {find_start(board), 0, EAST, ""};
    // Solve board
    bool success = solve(board, laser, target);
    // Trace board
    shoot(board, &laser, true);
    // Print traced board
    printf(success ? "Solution found!\n" : "Word cannot be constructed!\n");
    print_board(board);
    free_cells(board);
    return EXIT_SUCCESS;
}
#endif
