#include "data.hpp"

// notes: pieces[0][0] should be the white, LEFT rook. 
uint8_t pieces[64];
// uint8_t init[64] = {
//     wR, wN, wB, wQ, wK, wB, wN, wR,
//     wP, wP, wP, wP, wP, wP, wP, wP, 
//     0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
//     0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
//     0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
//     0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
//     bP, bP, bP, bP, bP, bP, bP, bP, 
//     bR, bN, bB, bQ, bK, bB, bN, bR
// };
// testing
uint8_t init[64] = {
    wR, 0 , 0 , 0 , wK, 0 , 0 , wR,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , bP, 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    bR, 0 , 0 , bK, 0 , 0 , 0 , bR
};

uint64_t bb_0;
uint64_t bb_90;
uint32_t ply;
uint8_t side;