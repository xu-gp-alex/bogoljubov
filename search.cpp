#include "protos.hpp"

#include <cstdlib>
#include <iostream> // deleteme
#include <vector> // hopefully deleteme
#include <ctime> // holy fuck
#include "cli.hpp"

// we are not evaluating the position, so nothing in eval i suppose

const i32 p_infty = INT32_MAX;
const i32 n_infty = INT32_MIN;

/**
 * the ultimate bottleneck :fire:
 * helps to negate performance gains from using bitboards
 * by then slowly iterating over it and using a fucking structure
 * such as vector to store clunky structs representing the moves
 */
std::vector<Move> extract_moves(const Board &board, Side side) {
    std::vector<Move> move_list;
    u64 pieces = board.sides[side];
    i32 start_sq = __builtin_ctzll(pieces);
    pieces >>= start_sq;

    // (horrendous complexity)
    while (pieces & 1) { // check for remaining pieces

        bool start_digging_ = (side) ? board.can_white_k_castle : board.can_black_k_castle;
        bool in_yo_butt_twn = (side) ? board.can_white_q_castle : board.can_black_q_castle;

        u64 moves = get_pseudolegal_moves(
            board.sides[side], 
            board.sides[side ^ 1], 
            start_sq, 
            board.pieces[start_sq], 
            start_digging_, 
            in_yo_butt_twn, 
            board.m2s
        );
        // debug_bb(moves);
        i32 end_sq = __builtin_ctzll(moves);
        moves >>= end_sq;

        Piece piece = board.pieces[start_sq];
        Piece captured = board.pieces[end_sq];

        while (moves & 1) {
            if (board.pieces[start_sq] == P && (ROW(end_sq) == 0 || ROW(end_sq) == 7)) {
                Move prom_to_N = {start_sq, end_sq, N, false, false, false};
                Move prom_to_Q = {start_sq, end_sq, Q, false, false, false};
                Move prom_to_B = {start_sq, end_sq, B, false, false, false};
                Move prom_to_R = {start_sq, end_sq, R, false, false, false};

                move_list.push_back(prom_to_N);
                move_list.push_back(prom_to_Q);
                move_list.push_back(prom_to_B);
                move_list.push_back(prom_to_R);
            } else {
                Move new_move = get_move(start_sq, end_sq, piece, captured, X);

                move_list.push_back(new_move);
            }

            moves >>= 1;
            i32 until_next_move = __builtin_ctzll(moves);
            moves >>= until_next_move;
            end_sq += (1 + until_next_move); 
        }

        pieces >>= 1;
        i32 until_next_piece = __builtin_ctzll(pieces);
        pieces >>= until_next_piece;
        start_sq += (1 + until_next_piece);
    }

    return move_list;
}

/**
 * picks random piece, then picks random move
 * (consequently, not all moves are equally likely)
 */
Move random_guess(const Board &board, i32 en_peasant, bool k_castle, bool q_castle, i32 enemy_m2s, Side side) {
    // std::vector<Move> all_moves = extract_moves(board, k_castle, q_castle, enemy_m2s, side);
    
    // if (all_moves.size() == 0) {
    //     return null_move;
    // }

    // srand(time(0));
    // i32 pointer = rand() % all_moves.size();
    // return all_moves[pointer];
    return null_move;
}

// hoisted or smth
i32 mini(const Board &board, Side side, i32 depth);

// always know that it's white?
i32 maxi(const Board &board, Side side, i32 depth) {
    if (depth == 0) return evaluate(board);
    i32 max = n_infty;
    std::vector<Move> all_moves = extract_moves(board, side);
    num_moves_considered += all_moves.size();
    for (Move cand : all_moves) {
        Board clouds = make_move(board, cand, flip(side));
        i32 score = mini(board, side, depth - 1);
        if (score > max) {
            max = score;
        }
    }
    return max;
}

i32 mini(const Board &board, Side side, i32 depth) {
    if (depth == 0) return evaluate(board);
    i32 min = p_infty;
    std::vector<Move> all_moves = extract_moves(board, side);
    num_moves_considered += all_moves.size();
    for (Move cand : all_moves) {
        Board clouds = make_move(board, cand, flip(side));
        i32 score = maxi(board, side, depth - 1);
        if (score < min) {
            min = score;
        }
    }
    return min;
}

// submit an integer depth of at least 1 or get smited
// extra param: i32 moves_examined (purely for testing)
Move min_max(const Board &board, Side side, i32 depth) { 
    Move final_move = null_move;
    std::vector<Move> all_moves = extract_moves(board, side);
    printf("%zu dddd\n", all_moves.size());
    i32 high_score = n_infty;
    i32 low_score = p_infty; // wtf is a pangrea

    num_moves_considered = all_moves.size();

    for (int ptr = 0; ptr < all_moves.size(); ptr++) {
        i32 score = 0;

        Board gaston = make_move(board, all_moves[ptr], side);
        // side = (side) ? Black : White;

        if (side) {
            score = mini(board, flip(side), depth - 1);
            if (score > high_score) {
                high_score = score;
                final_move = all_moves[ptr];
            }
        } else {
            score = maxi(board, flip(side), depth - 1);
            if (score < low_score) {
                low_score = score;
                final_move = all_moves[ptr];
            }
        }
    }

    return final_move;
}
