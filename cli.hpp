#pragma once

#include <iostream>
#include <stdint.h>
#include <map>
#include <string>

#include "data.hpp"
#include "def.hpp"
#include "board.hpp"

extern std::map<uint8_t, std::string> itoa;
extern void print_board();
extern void user_move(std::string curr);