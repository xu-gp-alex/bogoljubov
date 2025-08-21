#pragma once

#include "data.hpp"
#include "cli.hpp" // DELETEME

#include <vector>

inline i32 ROW(i32 sq) { return sq >> 3; }
inline i32 COL(i32 sq) { return sq & 7; }

/* board.cpp */

extern void init_board();
extern Board get_new_board();

/* attack generation */

extern u64 get_white_pawn_pushes(i32 square, u64 occupancy);
extern u64 get_white_pawn_captures(i32 square);
extern u64 get_black_pawn_pushes(i32 square, u64 occupancy);
extern u64 get_black_pawn_captures(i32 square);
extern u64 get_knight_moves(i32 square);
extern u64 get_king_moves(i32 square);
extern u64 get_bishop_moves(i32 square, u64 occupancy);
extern u64 get_rook_moves(i32 square, u64 occupancy);
extern u64 get_queen_moves(i32 square, u64 occupancy);

extern u64 case_k_castle(i32 square, u64 occupancy); // evil
extern u64 case_q_castle(i32 square, u64 occupancy); // evil
extern u64 case_en_passant(i32 square, u64 pawns, i32 en_peasant); // evil

// k_castle and q_castle are redundant currently

extern u64 get_moves(const Board &board, i32 square, i32 en_peasant, bool k_castle, bool q_castle, Side side); // evil

extern bool is_move_legal(const Board &board, i32 start, i32 end, i32 en_peasant, bool k_castle, bool q_castle, Side side, Piece promotion); // evil
extern bool is_check(i32 square, u64 occupancy, u64 s_p, u64 d_p, u64 h_p, u64 p_p, Side side);
// extern i32 in_check()... for double checks

extern Board exec_en_passant(const Board &board, i32 start, i32 end, i32 en_peasant); // evil
extern Board exec_k_castle(const Board &board, i32 start); // evil
extern Board exec_q_castle(const Board &board, i32 start); // evil
extern Board exec_promotion(const Board &board, i32 start, i32 end, Piece promotion, Side side); // prolly evil
extern Board make_move(const Board &board, i32 start, i32 end, i32 en_peasant, bool k_castle, bool q_castle, Side side, Piece promotion); // evil

extern void generate_rook_magics(); // (MUST BE CALLED BEFORE `init_board`)
extern void generate_bishop_magics(); // (MUST BE CALLED BEFORE `init_board`)

/* search.cpp */

extern std::vector<move> extract_moves(const Board& board, bool k_castle, bool q_castle, Side side);
extern move random_guess(const Board &board, i32 en_peasant, bool k_castle, bool q_castle, Side side);

/* eval.cpp */

extern i32 evaluate(const Board &board);