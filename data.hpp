#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "def.hpp"

extern uint8_t pieces[64];
extern uint8_t init[64];
extern uint64_t bb_0;
// todo: write functions to just "rotate" the first bitboard?
extern uint64_t bb_90;
extern uint32_t ply;
extern uint8_t side;
