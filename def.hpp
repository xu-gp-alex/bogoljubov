#pragma once

#include <cstdint>

// deleteme
#define wP 0x1
#define wN 0x2
#define wK 0x3
#define wQ 0x4
#define wB 0x5
#define wR 0x6

// deleteme
#define bP 0x7
#define bN 0x8
#define bK 0x9
#define bQ 0xa
#define bB 0xb
#define bR 0xc

// define some useful squares
// #define A1 0
// #define H1 7
// #define A8 56
// #define H8 63

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