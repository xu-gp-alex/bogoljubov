#include "protos.hpp"

#include <cstdlib>
#include <iostream> // deleteme
#include <vector> // hopefully deleteme
#include <ctime> // holy fuck

// we are not evaluating the position, so nothing in eval i suppose

/**
 * the ultimate bottleneck :fire:
 * helps to negate performance gains from using bitboards
 * by then slowly iterating over it and using a fucking structure
 * such as vector to store clunky structs representing the moves
 */
std::vector<move> extract_moves(const Board &board, bool k_castle, bool q_castle, Side side) {
    std::vector<move> move_list;
    u64 pieces = board.sides[side];
    i32 start_sq = __builtin_ctzll(pieces);
    pieces >>= start_sq;

    // (horrendous complexity)
    while (pieces & 1) { // check for remaining pieces
        u64 moves = get_moves(board, start_sq, en_peasant, k_castle, q_castle, side);
        i32 end_sq = __builtin_ctzll(moves);
        moves >>= end_sq;

        while (moves & 1) {
            if (board.pieces[start_sq] == P && (ROW(end_sq) == 0 || ROW(end_sq) == 7)) {
                move prom_to_N = {start_sq, end_sq, N, false, false};
                move prom_to_Q = {start_sq, end_sq, Q, false, false};
                move prom_to_B = {start_sq, end_sq, B, false, false};
                move prom_to_R = {start_sq, end_sq, R, false, false};

                move_list.push_back(prom_to_N);
                move_list.push_back(prom_to_Q);
                move_list.push_back(prom_to_B);
                move_list.push_back(prom_to_R);
            } else {
                move new_move = {start_sq, end_sq, X, false, false};

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
move random_guess(const Board &board, i32 en_peasant, bool k_castle, bool q_castle, Side side) {
    std::vector<move> all_moves = extract_moves(board, k_castle, q_castle, side);
    
    if (all_moves.size() == 0) {
        return null_move;
    }

    srand(time(0));
    i32 pointer = rand() % all_moves.size();
    return all_moves[pointer];
}

i32 alphaBetaMin(const Board &board, i32 alpha, i32 beta, i32 depthLeft);

/**
 * (ripped straight from cpw)
 * currently has dogshit performance, bc so many goddamn board structs
 */
i32 alphaBetaMax(const Board &board, i32 alpha, i32 beta, i32 depthleft) {
    if (depthleft == 0) return evaluate(board);

    std::vector<move> all_moves = extract_moves(board, false, false, White); // fix plz
    for (move cand : all_moves) {
        Board next_board = make_move(
            board, 
            cand.start, 
            cand.end, 
            -1, // fix plz
            cand.k_castle, 
            cand.q_castle, 
            White, // fix plz
            cand.promote
        );
        i32 score = alphaBetaMin(next_board, alpha, beta, depthleft - 1);

        if( score >= beta )
            return beta;   // fail hard beta-cutoff
        if( score > alpha )
            alpha = score; // alpha acts like max in MiniMax
    }
    return alpha;
}

/**
 * (also ripped straight from cpw)
 * currently has dogshit performance, bc so many goddamn board structs
 */
i32 alphaBetaMin(const Board &board, i32 alpha, i32 beta, i32 depthleft) {
//    if (depthleft == 0) return -evaluate(board);
//    for ( all moves) {
//       score = alphaBetaMax( alpha, beta, depthleft - 1 );
//       if( score <= alpha )
//          return alpha; // fail hard alpha-cutoff
//       if( score < beta )
//          beta = score; // beta acts like min in MiniMax
//    }
//    return beta;
    return 0;
}