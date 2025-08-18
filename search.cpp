#include "protos.hpp"

#include <cstdlib>
#include <iostream> // deleteme
#include <vector> // hopefully deleteme
#include <ctime> // holy fuck

// we are not evaluating the position, so nothing in eval i suppose

/**
 * picks random piece, then picks random move
 * (consequently, not all moves are equally likely)
 */
move random_guess(const Board &board, i32 en_peasant, bool k_castle, bool q_castle, Side side) {
    u64 can_move = board.sides[side];

    i32 square = __builtin_ctzll(can_move);
    can_move >>= square;

    // horrendous complexity
    // literally just get this done...
    std::vector<move> all_moves;
    while (can_move & 1) { // is there still a piece left?
        u64 moves = get_moves(board, square, en_peasant, k_castle, q_castle, side);

        i32 ending_sq = __builtin_ctzll(moves);
        moves >>= ending_sq;

        while (moves & 1) {
            move new_move;
            new_move.start = square;
            new_move.end = ending_sq;
            new_move.promote = X; // xdddddddd

            // below literally does not matter, just for kicks
            new_move.k_castle = false;
            new_move.q_castle = false;

            all_moves.push_back(new_move);

            i32 mega_candy = __builtin_ctzll(moves) + 1;
            moves >>= mega_candy;
            ending_sq += mega_candy;
        }

        i32 gooner = __builtin_ctzll(can_move) + 1;
        can_move >>= gooner;
        square += gooner;
    }

    // we need a null move convention
    move null_move;
    null_move.start = -1;
    null_move.end = -1;
    null_move.promote = X;
    null_move.k_castle = false;
    null_move.q_castle = false;

    if (all_moves.size() == 0) {
        return null_move;
    }

    srand(time(0));

    i32 pointer = rand() % all_moves.size();
    // std::cout << "picked the " << pointer << " move\n";
    // std::cout << all_moves.size() << " moves to choose from\n";
    return all_moves[pointer];
}