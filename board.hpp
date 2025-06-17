#pragma once

// notes: better way to store moves than vector?
#include <vector>

#include "data.hpp"
#include "def.hpp"

extern bool make_move(move curr);
extern void init_board();
// notes: prioritize checks/captures/attacks?
// notes: is this memory-efficient? the structs?
// todo: change back to vector of moves
extern void get_captures();
// extern std::vector<move> get_moves();
// extern move get_rand_move();