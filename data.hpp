#pragma once

#include "def.hpp"

extern u8 pieces[64];
extern const u8 init[64];
extern const u8 test[64];

extern u64 board[6]; // board --> boards

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
// todo: what is the point of data.hpp?
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
// todo: cataclysmic change but white should probably be 1
//       lots of !side everywhere in the fucking code
extern u8 side; // 0=white, 1=black

extern i32 white_en_pessant_tgt;
extern i32 black_en_pessant_tgt;

extern bool w_king_moved;
extern bool a1_rook_moved;
extern bool h1_rook_moved;
extern bool b_king_moved;
extern bool a8_rook_moved;
extern bool h8_rook_moved;