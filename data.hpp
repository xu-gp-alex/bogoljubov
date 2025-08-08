#pragma once

#include <cstdint>

// todo: figure out which header files I should move into the cpp files
#include "def.hpp"

// notes: i believe this belongs here bc def.hpp currently
//        does not need these cool ass integer types
// notes: do the fancy spacing (properly) such that i32 and 
//        u64 would be vertically aligned
typedef std::int8_t      i8;
typedef std::int32_t     i32;
typedef std::int64_t     i64;
typedef std::uint8_t     u8;
typedef std::uint32_t    u32;
typedef std::uint64_t    u64;

// notes: the fucking enums are just implicitly ints, how to fix later...
extern u8 pieces[64];
extern const u8 init[64];
// testing
// extern u8 test[64];
extern u64 board[12];

// todo: refine so from single struct "board"
//       can access white/black, specific pieces, etc.
extern u64 white_pieces;
extern u64 black_pieces;
extern u64 pawns;
extern u64 rooks;
extern u64 knights;
extern u64 bishops;
extern u64 queens;
extern u64 kings;

extern u64 prev_board[8];

// todo: should hardcode masks?
extern const u64 rook_masks[64];
extern const u64 bishop_masks[64];
extern const u64 knight_masks[64];
extern const u64 king_masks[64];
// todo: decide whether to hardcode magics, currently
//       will generate for each game
// todo: perhaps incorporate more efficient magics,
//       such as the fixed ones and low-bit counts
extern u64 rook_magics[64];
extern u64 bishop_magics[64];

extern const u64 white_pawn_captures[64];
extern const u64 black_pawn_captures[64];

// todo: should this fixed? if so, do you just copy a fucking
//       arr[64][1 << 12] array directly into the .cpp file?
extern u64 rook_moves[64][1 << 12];
extern u64 bishop_moves[64][1 << 9];

extern u32 ply;
extern u8 side;
