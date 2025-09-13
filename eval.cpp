#include "protos.hpp"

/**
 * currently just checks material count
 */
i32 evaluate(const Board &board) {
    // worst code ever
    i32 wP = __builtin_popcountll(board.sides[White] & board.pieces_bb[P]);
    i32 bP = __builtin_popcountll(board.sides[Black] & board.pieces_bb[P]);
    i32 wN = __builtin_popcountll(board.sides[White] & board.pieces_bb[N]);
    i32 bN = __builtin_popcountll(board.sides[Black] & board.pieces_bb[N]);
    i32 wK = __builtin_popcountll(board.sides[White] & board.pieces_bb[K]);
    i32 bK = __builtin_popcountll(board.sides[Black] & board.pieces_bb[K]);
    i32 wQ = __builtin_popcountll(board.sides[White] & board.pieces_bb[Q]);
    i32 bQ = __builtin_popcountll(board.sides[Black] & board.pieces_bb[Q]);
    i32 wB = __builtin_popcountll(board.sides[White] & board.pieces_bb[B]);
    i32 bB = __builtin_popcountll(board.sides[Black] & board.pieces_bb[B]);
    i32 wR = __builtin_popcountll(board.sides[White] & board.pieces_bb[R]);
    i32 bR = __builtin_popcountll(board.sides[Black] & board.pieces_bb[R]);

    i32 p = (wP - bP) * 1;
    i32 n = (wN - bN) * 3;
    i32 k = (wK - bK) * 1000;
    i32 q = (wQ - bQ) * 9;
    i32 b = (wB - bB) * 3;
    i32 r = (wR - bR) * 5;

    return p + n + k + q + b + r;
}