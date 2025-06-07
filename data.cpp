#include "data.hpp"

// notes: pieces[0][0] should be the white, LEFT rook. 
uint8_t pieces[64];
uint8_t init[64] = {
    wR, wN, wB, wQ, wK, wB, wN, wR,
    wP, wP, wP, wP, wP, wP, wP, wP, 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
    bP, bP, bP, bP, bP, bP, bP, bP, 
    bR, bN, bB, bQ, bK, bB, bN, bR
};