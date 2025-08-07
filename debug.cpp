#include "protos.hpp"

// todo: (IMPORTANT) make a more robust way in general to test the functions
// todo: perhaps tell the user to run this (similar to run "set_hash" before hash in the tscp)
// todo: fucking learn to debug beyond debug statements kms kms kms
#include <string>
#include <iostream>
std::string str[] = {
    "0000", "0001", "0010", "0011", 
    "0100", "0101", "0110", "0111", 
    "1000", "1001", "1010", "1011", 
    "1100", "1101", "1110", "1111",
};

void debug_u64(u64 curr) {
    std::cout << "0b ";
    for (int i = 15; i >= 0; i--) {
        std::cout << str[(curr >> (4 * i)) & 0xF] << ' ';
    }
    std::cout << std::endl;
}

std::string rev_str[] = {
    "0000", "1000", "0100", "1100", 
    "0010", "1010", "0110", "1110", 
    "0001", "1001", "0101", "1101", 
    "0011", "1011", "0111", "1111",
};

void debug_board(u64 curr) {
    for (int i = 15; i >= 0; i-=2) {
        std::cout << rev_str[(curr >> (4 * (i-1))) & 0xF];
        std::cout << rev_str[(curr >> (4 * i)) & 0xF];
        std::cout << '\n';
    }
    std::cout << '\n';
}