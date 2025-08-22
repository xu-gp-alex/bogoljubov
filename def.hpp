#pragma once

#include <cstdint>

// useful squares
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
    X=-1, P, N, K, Q, B, R, // replace P with wP and bP
};

enum Side {
    Black, White
};

typedef struct {
    i32 start;
    i32 end;

    // unique *types* of moves
    Piece promote;
    bool k_castle; 
    bool q_castle; 
    bool en_peasant; // --> sq i32 instead..??
} Move;