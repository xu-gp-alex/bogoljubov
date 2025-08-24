#pragma once

#include "data.hpp"
// #include "cli.hpp" // DELETEME

#include <vector>

inline i32 ROW(i32 sq) { return sq >> 3; }
inline i32 COL(i32 sq) { return sq & 7; }

/* board.cpp */

extern void init_data();
extern Board get_new_board();
extern Board get_custom_board(std::string setup);

/* attack generation */

extern u64 get_white_pawn_pushes(i32 square, u64 occupancy);
extern u64 get_white_pawn_captures(i32 square); // redundant?
extern u64 get_black_pawn_pushes(i32 square, u64 occupancy);
extern u64 get_black_pawn_captures(i32 square); // redundant?
extern u64 get_knight_moves(i32 square); // redundant?
extern u64 get_king_moves(i32 square); // redundant?
extern u64 get_bishop_moves(i32 square, u64 occupancy);
extern u64 get_rook_moves(i32 square, u64 occupancy);
extern u64 get_queen_moves(i32 square, u64 occupancy);

extern bool can_k_castle(i32 square, u64 occupancy); 
extern bool can_q_castle(i32 square, u64 occupancy); 
extern bool can_en_peasant(i32 square, i32 enemy_m2s); 
extern u64 get_pseudolegal_moves(u64 friendly, u64 enemy, i32 square, Piece piece, bool k_castle_possible, bool q_castle_possible, i32 enemy_m2s);

extern bool can_promote(i32 square, Side side);
extern bool is_move_legal(const Board &board, Move m, Side side, bool k_castle_possible, bool q_castle_possible, i32 enemy_m2s);

extern bool is_check(i32 square, u64 occupancy, u64 enemy_R_or_Q, u64 enemy_B_or_Q, u64 enemy_N, u64 enemy_wP, u64 enemy_bP, Side side);

extern Board make_move(const Board &board, Move m, Side side); 

extern Move get_move(i32 start, i32 end, Piece piece, Piece captured, Piece promote);
extern void update_global_states(Move m, Piece piece, Piece captured, Side side);

// get rid of these man...
extern void generate_rook_magics(); // (MUST BE CALLED BEFORE `init_board`)
extern void generate_bishop_magics(); // (MUST BE CALLED BEFORE `init_board`)

/* search.cpp */

extern std::vector<Move> extract_moves(const Board& board, bool k_castle, bool q_castle, i32 enemy_m2s, Side side);
extern Move random_guess(const Board &board, i32 en_peasant, bool k_castle, bool q_castle, i32 enemy_m2s, Side side);
extern Move min_max(const Board &board, i32 en_peasant, bool k_castle, bool q_castle, i32 enemy_m2s, Side side);

/* eval.cpp */

extern i32 evaluate(const Board &board);