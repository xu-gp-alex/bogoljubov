#pragma once

#include <string>

#include "data.hpp"

extern void print_pieces(const Board &board);
extern void debug_pieces(const Board &board);
extern void debug_bbs(const Board &board);
extern void debug_bb(u64 curr);
extern void debug_bb(u64 curr, std::string label);

// bad coding practice again??
extern bool valid_str(std::string input);
// extern bool str_to_move(std::string pgn_input)
// extern bool str_to_move(std::string fen_input)
extern move str_to_move(std::string input);