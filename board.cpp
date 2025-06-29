#include "board.hpp"

/* helper functions */

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

i32 rook_dir[4] = {8, -8, 1, -1}; // N, S, E, W
/**
 * returns u64 representing moves of a rook
 * given its current square and surrounding pieces
 */
u64 f_rook(i32 square, u64 occupancy) {
    u64 res = 0;
    for (i32 dir = 0; dir < 4; dir++) {
        if (
            (dir == 0 && square / 8 < 7) || 
            (dir == 1 && square / 8 > 0) || 
            (dir == 2 && square % 8 < 7) || 
            (dir == 3 && square % 8 > 0)
        ) {
            i32 curr_square = square;

            do {
                curr_square += rook_dir[dir];
                res |= ((u64) 1 << curr_square);
            } while (
                (~occupancy >> curr_square) & 1 && 
                ((dir == 0 && curr_square / 8 < 7) || 
                (dir == 1 && curr_square / 8 > 0) || 
                (dir == 2 && curr_square % 8 < 7) || 
                (dir == 3 && curr_square % 8 > 0))
            );
        }
    }
    return res;
}

i32 bishop_dir[4] = {9, -7, -9, 7}; // NE, SE, SW, NW
/**
 * returns u64 representing moves of a bishop
 * given its current square and surrounding pieces
 */
u64 f_bishop(i32 square, u64 occupancy) {
    u64 res = 0;
    for (i32 dir = 0; dir < 4; dir++) {
        if (
            (dir == 0 && square / 8 < 7 && square % 8 < 7) || 
            (dir == 1 && square / 8 > 0 && square % 8 < 7) || 
            (dir == 2 && square / 8 > 0 && square % 8 > 0) || 
            (dir == 3 && square / 8 < 7 && square % 8 > 0) 
        ) {
            i32 curr_square = square;

            do {
                curr_square += bishop_dir[dir];
                res |= ((u64) 1 << curr_square);
            } while (
                (~occupancy >> curr_square) & 1 && 
                ((dir == 0 && curr_square / 8 < 7 && curr_square % 8 < 7) || 
                (dir == 1 && curr_square / 8 > 0 && curr_square % 8 < 7) || 
                (dir == 2 && curr_square / 8 > 0 && curr_square % 8 > 0) || 
                (dir == 3 && curr_square / 8 < 7 && curr_square % 8 > 0))
            );
        }
    }
    return res;
}

// todo: more better-er way of doing ts??
#include <cstdlib>
u64 rand_u64() {
    u64 hi = (u64) rand();
    u64 lo = (u64) rand();
    return (hi << 32) | lo;
}

bool try_rook_magic(u64 square, u64 magic) {
    u64 attack_set = rook_masks[square];
        
    for (u64 p = 0; p < (1 << __builtin_popcountll(attack_set)); p++) {
        u64 subset = 0;
        i32 p_bit = 0;
        
        // todo: find a better way to get the subsets of 
        //       given attack set
        for (i32 d = 0; d < 64; d++) {
            if ((attack_set >> d) & 0x1) {
                u64 resultant = (u64) (((p >> p_bit) & 0x1) << d);
                subset |= resultant;
                p_bit++;
            }
        }

        // todo: i32 vs u32?
        i32 index = (i32) ((subset * magic) >> (64 - 12));

        if (rook_moves[square][index] == 0) {
            rook_moves[square][index] = f_rook(square, subset);
        } else {
            if (f_rook(square, subset) != rook_moves[square][index]) {
                return false;
            }
        }
    }

    return true;
}

bool try_bishop_magic(u64 square, u64 magic) {
    u64 attack_set = bishop_masks[square];
        
    for (u64 p = 0; p < (1 << __builtin_popcountll(attack_set)); p++) {
        u64 subset = 0;
        i32 p_bit = 0;
        
        // todo: find a better way to get the subsets of 
        //       given attack set
        for (i32 d = 0; d < 64; d++) {
            if ((attack_set >> d) & 0x1) {
                u64 resultant = (u64) (((p >> p_bit) & 0x1) << d);
                subset |= resultant;
                p_bit++;
            }
        }

        // todo: i32 vs u32?
        i32 index = (i32) ((subset * magic) >> (64 - 9));

        if (bishop_moves[square][index] == 0) {
            bishop_moves[square][index] = f_bishop(square, subset);
        } else {
            if (f_bishop(square, subset) != bishop_moves[square][index]) {
                return false;
            }
        }
    }

    return true;
}

/* board.cpp */

// todo: make illegal moves illegal
bool make_move(move curr) {
    pieces[curr.to] = pieces[curr.from];
    pieces[curr.from] = 0;
    ply++;
    
    // notes: works?
    side = ~side;
    return true;
}

void init_board() {
    for (int i = 0; i < 64; i++) {
        // testing
        // pieces[i] = init[i];
        pieces[i] = test[i];
    }

    white_pieces = 0xffff000000000000;
    black_pieces = 0x000000000000ffff;

    ply = 0;
    side = 0;
}

// notes: insanely retarded method
// testing
#include <iostream>
void get_captures() {
    // notes: perhaps just iterate through a *list* of white/black pieces?
    uint8_t mask = (side) ? 0x0 : 0;
    for (int i = 0; i < 64; i++) {
        if (pieces[i] != 0) {
            // testing
            std::cout << i << ' ';
            printf("%x ", mask);
        }
    }
    std::cout << '\n';
}

// notes: insanely retarded method
// std::vector<move> get_moves() {

// }

// notes: insanely retarded method
// notes: supposed to use Zobrist something?
// move get_rand_move() {

// }

// notes: function signature inspired by pradu kannan
// notes: proper data types for inputs (uint8_t??)
// todo: fucking inline??

u64 get_rook_moves(i32 square, u64 occupancy) {
    occupancy &= rook_masks[square];
    occupancy *= rook_magics[square];
    occupancy >>= (64 - 12);

    return rook_moves[square][occupancy];
}

u64 get_bishop_moves(i32 square, u64 occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magics[square];
    occupancy >>= (64 - 9);

    return bishop_moves[square][occupancy];
}

// todo: probably delete later; looks redundant
// u64 get_knight_moves(i32 square, u64 occupancy) {
//     return knight_masks[square];
// }

// notes: need for each piece type?? how to efficiently split later?
// u64 get_queen_moves(uint8_t square, u64 occupancy) {
//     return get_rook_moves(square, occupancy) | get_bishop_moves(square, occupancy);
// }

void generate_rook_magics() {
    for (i32 square = 0; square < 64; square++) {
        u64 magic = 0;
        do {
            for (int j = 0; j < (1 << 12); j++) {
                rook_moves[square][j] = 0;
            }

            // notes: figure out how did they arrive at multiply 3 times?
            magic = rand_u64() & rand_u64() & rand_u64();
        } while (!try_rook_magic(square, magic));

        rook_magics[square] = magic;
    }
}

void generate_bishop_magics() {
    for (i32 square = 0; square < 64; square++) {
        u64 magic = 0;
        do {
            for (int j = 0; j < (1 << 9); j++) {
                bishop_moves[square][j] = 0;
            }

            // notes: figure out how did they arrive at multiply 3 times?
            magic = rand_u64() & rand_u64() & rand_u64();
        } while (!try_bishop_magic(square, magic));

        bishop_magics[square] = magic;
    }
}
