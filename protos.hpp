#pragma once

// eventually de-couple this somehow
#include "data.hpp"

/* board.cpp */

// reddit suggested noexcept
inline i32 ROW(i32 sq) { return sq >> 3; }
inline i32 COL(i32 sq) { return sq & 7; }

extern void init_board();
extern Board get_new_board();

/* attack generation */

extern u64 get_white_pawn_pushes(i32 square, i32 occupancy);
extern u64 get_white_pawn_captures(i32 square);
extern u64 get_white_pawn_moves(i32 square, i32 occupancy);
extern u64 get_black_pawn_pushes(i32 square, i32 occupancy);
extern u64 get_black_pawn_captures(i32 square);
extern u64 get_black_pawn_moves(i32 square, i32 occupancy);
extern u64 get_knight_moves(i32 square);
extern u64 get_king_moves(i32 square);
extern u64 get_bishop_moves(i32 square, u64 occupancy);
extern u64 get_rook_moves(i32 square, u64 occupancy);
extern u64 get_queen_moves(i32 square, u64 occupancy);

// add more consts, consts are awesome
// could use templating?

// get_moves --> get_legal_moves()?
extern u64 get_white_pawn_moves(const Board &board, i32 square, i32 en_peasant);
extern u64 get_black_pawn_moves(const Board &board, i32 square, i32 en_peasant);
extern u64 get_moves(const Board &board, i32 square, Piece piece, Side side, i32 en_peasant);
extern u64 get_captures(const Board &board, i32 square, Piece piece, Side side, i32 en_peasant);;

extern bool is_move_legal(const Board &board, i32 start, i32 end, Piece piece, Side side, i32 en_peasant);
extern bool is_castle_legal();
extern bool is_en_pessant_legal();
extern bool is_promotion_legal();

extern bool is_check(i32 square, u64 occupancy, u64 s_p, u64 d_p, u64 h_p, u64 p_p, Side side);
// extern i32 in_check()... for double checks

extern Board make_move(const Board &board, i32 start, i32 end, Piece piece, Side side, i32 en_peasant);

extern void exec_move();
extern void exec_castle(bool kingside, u8 side);
extern void exec_en_pessant(i32 start, i32 end, i32 piece_type, u8 side); // arguments??
extern void exec_promotion(i32 start, Piece promote_to, u8 side); // need end?

// notes: should a function to generate_magics belong here??
// notes: benchmark this eventually

extern void generate_rook_magics(); // (MUST BE CALLED BEFORE `init_board`)
extern void generate_bishop_magics(); // (MUST BE CALLED BEFORE `init_board`)