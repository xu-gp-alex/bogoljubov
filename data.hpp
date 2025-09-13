#pragma once

#include "def.hpp"

extern const Piece init[64];

// move to def.hpp?
struct Board {
    Piece pieces[64];
    u64 pieces_bb[6];
    u64 sides[2];

    i32 m2s;
    bool can_white_k_castle;
    bool can_white_q_castle;
    bool can_black_k_castle;
    bool can_black_q_castle;
};

// extern Board global_board;

extern u32 ply;
extern Side side; // 0=black, 1=white
extern bool decisive_result; // make this express more results

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

extern u64 num_moves_considered;

extern const Move null_move;