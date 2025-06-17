#include "board.hpp"

// todo: public v private
// todo: reference or what
uint8_t rev(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// todo: make illegal moves illegal
bool make_move(move curr) {
    pieces[curr.to] = pieces[curr.from];
    pieces[curr.from] = 0;
    ply++;
    
    // notes: works?
    side = ~side;
    return true;
}

void init_board() {
    for (int i = 0; i < 64; i++) {
        pieces[i] = init[i];
    }

    bb_0  = 0xFFFF00000000FFFF;
    bb_90 = 0xC3C3C3C3C3C3C3C3;

    ply = 0;
    side = 0;
}

// notes: insanely retarded method
// testing
#include <iostream>
void get_captures() {
    // notes: perhaps just iterate through a *list* of white/black pieces?
    uint8_t mask = (side) ? 0x0 : 0;
    for (int i = 0; i < 64; i++) {
        if (pieces[i] != 0) {
            // testing
            std::cout << i << ' ';
            printf("%x ", mask);
        }
    }
    std::cout << std::endl;
}

// notes: insanely retarded method
// std::vector<move> get_moves() {

// }

// notes: insanely retarded method
// notes: supposed to use Zobrist something?
// move get_rand_move() {

// }