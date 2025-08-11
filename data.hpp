#pragma once

#include "def.hpp"

/**
 * for reference, pieces[0] is a1 and pieces[63] is h8
 */
extern u8 pieces[64];
extern Piece pieces[64];
extern const u8 init[64];
extern const u8 test[64];

// typedef??
struct Board {
    u64 pieces[6];
    u64 sides[2];
};

// extern Board global_board;

extern u32 ply;
extern Side side; // 0=black, 1=white
extern i32 en_peasant;

// extern <datatype> castle;
extern bool w_king_moved;
extern bool a1_rook_moved;
extern bool h1_rook_moved;
extern bool b_king_moved;
extern bool a8_rook_moved;
extern bool h8_rook_moved;

extern const u64 rook_masks[64];
extern const u64 bishop_masks[64];
extern const u64 knight_masks[64];
extern const u64 king_masks[64];
extern const u64 white_pawn_captures[64];
extern const u64 black_pawn_captures[64];

extern u64 rook_magics[64];
extern u64 bishop_magics[64];
extern u64 rook_moves[64][1 << 12];
extern u64 bishop_moves[64][1 << 9];