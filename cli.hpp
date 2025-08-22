#pragma once

#include <string>

#include "data.hpp"

extern void print_pieces(const Board &board);
extern void print_move(const Move &move);

extern void debug_pieces(const Board &board);
extern void debug_bbs(const Board &board);
extern void debug_bb(u64 curr);
extern void debug_bb(u64 curr, std::string label);

extern bool valid_str(std::string input);
// extern bool str_to_move(std::string pgn_input)
// extern bool str_to_move(std::string fen_input)
extern Move str_to_move(const Board &board, std::string input);