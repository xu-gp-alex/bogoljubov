#pragma once

#include <cstdint>

// define some useful squares
#define A1 0
#define E1 4
#define H1 7
#define A8 56
#define E8 60
#define H8 63

typedef std::int8_t      i8;
typedef std::int32_t     i32;
typedef std::int64_t     i64;
typedef std::uint8_t     u8;
typedef std::uint32_t    u32;
typedef std::uint64_t    u64;

enum Piece {
    X=-1, P, N, K, Q, B, R,
};

enum Side {
    Black, White
};

typedef struct {
    i32 start;
    i32 end;
    Piece promote;
    // readable booleans..
    bool k_castle; // if end-start==2
    bool q_castle; // if start-end==2

    // alternatives:
    // Piece piece; // not needed bc of pieces[]
    // bool side; // not intrinsic to a move
    // move_bits (from tom) // im stupid
} move;