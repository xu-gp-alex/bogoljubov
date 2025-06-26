#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "def.hpp"

extern uint8_t pieces[64];
extern uint8_t init[64];

extern uint64_t white_pieces;
extern uint64_t black_pieces;
extern const uint64_t rook_masks[64];
extern const uint64_t bishop_masks[64];

extern uint32_t ply;
extern uint8_t side;
