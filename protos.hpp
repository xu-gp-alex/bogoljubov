#pragma once

// notes: (IMPORTANT) SHOULD I RECONSOLIDATE THIS INTO 
//        PROTOS.HPP SIMILAR TO TSCP183, WHERE EVEN SEARCH
//        FUNCTIONS BELONG HERE????? AFTER ALL IS A CHESS ENGINE
//        NOT JUST CHESSBOARD!!
#include <cstdint>
#include <string>

#include "data.hpp"

typedef std::int8_t      i8;
typedef std::int32_t     i32;
typedef std::int64_t     i64;
typedef std::uint8_t     u8;
typedef std::uint32_t    u32;
typedef std::uint64_t    u64;

/* board.cpp */

// extern bool make_move(move curr);
// extern bool make_move(u64 from, u64 to);
extern void init_board();

// extern u64 get_rook_moves(i32 square, u64 occupancy);
// extern u64 get_bishop_moves(i32 square, u64 occupancy);
// extern u64 get_knight_moves(i32 square, u64 occupancy);

extern u64 get_legal_moves(i32 square, u64 occupancy, i32 piece_type, u8 side);
// extern u64 get_captures(i32 square, u64 occupancy, i32 piece_type, u8 side);
// extern u64 get_checks(i32 square, u64 occupancy, i32 piece_type, u8 side);

extern bool make_move(i32 start, i32 end, Piece piece, Piece promote_to, u8 side);
extern bool is_move_legal(i32 start, i32 end, Piece piece, Piece promote_to, u64 enemy_pieces, u8 side);
extern bool is_castle_legal();
extern bool is_en_pessant_legal();
extern bool is_promotion_legal();

// extern bool is_check(u8 side, u64 occupancy);

extern void exec_move(i32 start, i32 end, i32 piece_type, u8 side);
extern void exec_castle(bool kingside, u8 side);
extern void exec_en_pessant(i32 start, i32 end, i32 piece_type, u8 side); // arguments??
extern void exec_promotion(i32 start, Piece promote_to, u8 side); // need end?

// notes: should a function to generate_magics belong here??
// notes: benchmark this eventually

extern void generate_rook_magics(); // (MUST BE CALLED BEFORE `init_board`)
extern void generate_bishop_magics(); // (MUST BE CALLED BEFORE `init_board`)

/* debug.cpp */
// notes: for testing only, this is a horrible practice
void debug_u64(u64 curr);
void debug_board(u64 curr);
void debug_board(u64 curr, std::string label);