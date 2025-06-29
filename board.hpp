#pragma once

// notes: (IMPORTANT) SHOULD I RECONSOLIDATE THIS INTO 
//        PROTOS.HPP SIMILAR TO TSCP183, WHERE EVEN SEARCH
//        FUNCTIONS BELONG HERE????? AFTER ALL IS A CHESS ENGINE
//        NOT JUST CHESSBOARD!!
#include <cstdint>

#include "data.hpp"
#include "def.hpp"

typedef std::int8_t      i8;
typedef std::int32_t     i32;
typedef std::int64_t     i64;
typedef std::uint8_t     u8;
typedef std::uint32_t    u32;
typedef std::uint64_t    u64;

/* board.cpp */

extern bool make_move(move curr);
extern void init_board();
// notes: prioritize checks/captures/attacks?
// notes: is this memory-efficient? the structs?
// todo: change back to vector of moves
extern void get_captures();
// extern std::vector<move> get_moves();
// extern move get_rand_move();

extern u64 get_rook_moves(i32 square, u64 occupancy);
extern u64 get_bishop_moves(i32 square, u64 occupancy);

// notes: should a function to generate_magics belong here??
// notes: time this eventually

extern void generate_rook_magics(); // (MUST BE CALLED BEFORE `init_board`)

// notes: for testing only, this is a horrible practice
void debug_u64(u64 curr);
void debug_board(u64 curr);