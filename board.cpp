#include "board.hpp"

// todo: make illegal moves illegal
bool make_move(move curr) {
    pieces[curr.to] = pieces[curr.from];
    pieces[curr.from] = 0;
    return true;
}

void init_board() {
    for (int i = 0; i < 64; i++) {
        pieces[i] = init[i];
    }
}