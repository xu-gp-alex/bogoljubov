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

    // testing
    i32 pieces_considered = 0;

    // horrendous complexity
    // literally just get this done...
    std::vector<move> all_moves;
    while (can_move & 1) { // is there still a piece left?
        u64 moves = get_moves(board, square, en_peasant, k_castle, q_castle, side);
        // std::cout << "piece " << board.pieces[square] << ": \n";
        // debug_bb(moves);

        i32 ending_sq = __builtin_ctzll(moves);
        moves >>= ending_sq;

        // testing
        i32 bbbb = 0;

        // debug_bb(moves);

        while (moves & 1) {
            bbbb++;

            move new_move;
            new_move.start = square;
            new_move.end = ending_sq;
            new_move.promote = X; // xdddddddd

            // below literally does not matter, just for kicks
            new_move.k_castle = false;
            new_move.q_castle = false;

            bool freaky = false;
            if (board.pieces[square] == P && (ROW(ending_sq) == 0 || ROW(ending_sq) == 7)) {
                freaky = true;
            }

            if (!freaky) {
                all_moves.push_back(new_move);
            } else {
                move N_prom = {square, ending_sq, N, false, false};
                move Q_prom = {square, ending_sq, Q, false, false};
                move B_prom = {square, ending_sq, B, false, false};
                move R_prom = {square, ending_sq, R, false, false};

                all_moves.push_back(N_prom);
                all_moves.push_back(Q_prom);
                all_moves.push_back(B_prom);
                all_moves.push_back(R_prom);
            }

            moves >>= 1;
            i32 mega_candy = __builtin_ctzll(moves);
            moves >>= mega_candy;
            ending_sq += (1 + mega_candy); 

            // i32 mega_candy = __builtin_ctzll(moves) + 1;
            // moves >>= mega_candy;
            // ending_sq += mega_candy;
        }

        // std::cout << bbbb << " moves found\n";

        pieces_considered++;

        can_move >>= 1;
        i32 benson_boone = __builtin_ctzll(can_move);
        can_move >>= benson_boone;
        square += (1 + benson_boone);

        // i32 gooner = __builtin_ctzll(can_move) + 1;
        // can_move >>= gooner;
        // square += gooner;
    }

    std::cout << "considered " << pieces_considered << " pieces\n";

    // testing
    std::string kmsss = (side) ? "White" : "Black";
    std::cout << kmsss << " has " << all_moves.size() << " moves to choose from...\n";

    if (all_moves.size() == 0) {
        return null_move;
    }

    srand(time(0));

    i32 pointer = rand() % all_moves.size();
    std::cout << "from " << all_moves.size() << " moves, picked move " << pointer << "\n\n";
    return all_moves[pointer];
}