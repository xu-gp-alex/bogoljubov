#include <cstdlib>

#include "protos.hpp"

/* helper functions */
// todo: reorder ts...

// to be cpp idiomatic?
i32 ROW(i32 sq) { return sq >> 3; }
i32 COL(i32 sq) { return sq & 7; }

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

void init_board() {
    for (int i = 0; i < 64; i++) {
        pieces[i] = init[i];
    }

    ply = 0;
    side = White;

    en_peasant = -1;

    w_king_moved = false;
    a1_rook_moved = false;
    h1_rook_moved = false;
    b_king_moved = false;
    a8_rook_moved = false;
    h8_rook_moved = false;
}

// reference instead?? kms
Board get_new_board() {
    Board new_board;
    new_board.pieces[P] = 0x00ff00000000ff00ull;
    new_board.pieces[N] = 0x4200000000000042ull;
    new_board.pieces[K] = 0x1000000000000010ull;
    new_board.pieces[Q] = 0x0800000000000008ull;
    new_board.pieces[B] = 0x2400000000000024ull;
    new_board.pieces[R] = 0x8100000000000081ull;

    new_board.sides[Black] = 0xffff000000000000ull;
    new_board.sides[White] = 0x000000000000ffffull;
    return new_board;
}

// notes: proper data types for inputs (uint8_t??)
// todo: fucking inline??

/**
 * <function description>
 * (function signature inspired by pradu kannan)
 */
u64 get_rook_moves(i32 square, u64 occupancy) { // should take Board& instead?
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

u64 get_white_pawn_moves(const Board &board, i32 square, i32 en_peasant) {
    u64 occupancy = board.sides[White] | board.sides[Black];

    u64 captures = white_pawn_captures[square] & board.sides[Black];
    if (en_peasant >= 0 && en_peasant <= 7) {
        captures |= (0x10000000000ull << en_peasant);
    }

    u64 pushes = (1ull << (square + 8)) & ~occupancy;
    if (ROW(square) == 1 && pushes) {
        pushes |= (0x1000000ull << COL(square)) & ~occupancy;
    }

    return captures | pushes;
}

u64 get_black_pawn_moves(const Board &board, i32 square, i32 en_peasant) {
    u64 occupancy = board.sides[White] | board.sides[Black];

    u64 captures = black_pawn_captures[square] & board.sides[White];
    if (en_peasant >= 0 && en_peasant <= 7) {
        captures |= (0x10000ull << en_peasant);
    }

    u64 pushes = (1ull << (square - 8)) & ~occupancy;
    if (ROW(square) == 6 && pushes) {
        pushes |= ((0x100000000ull << COL(square)) & ~occupancy);
    }

    return captures | pushes;
}

// cleanliness, horrible argument naming
// legal_moves vs basic "moves"
u64 get_moves(const Board &board, i32 square, Piece piece, Side side, i32 en_peasant) {
    u64 occupancy = board.sides[Black] | board.sides[White];
    u64 friendly = board.sides[side];
    
    switch (piece) {
        case P:
            if (side) {
                return get_white_pawn_moves(board, square, en_peasant);
            } else {
                return get_black_pawn_moves(board, square, en_peasant);
            }

        case N:
            return knight_masks[square] & ~friendly;

        case K:
            return king_masks[square] & ~friendly;

        case Q:
            return (get_bishop_moves(square, occupancy) | 
                    get_rook_moves(square, occupancy)) & ~friendly;

        case B:
            return get_bishop_moves(square, occupancy) & ~friendly;

        case R:
            return get_rook_moves(square, occupancy) & ~friendly;

        default:
            break;
    }

    return 0;
}

u64 get_captures(const Board &board, i32 square, Piece piece, Side side, i32 en_peasant) {
    u64 enemy = board.sides[side ^ 1];
    return get_moves(board, square, piece, side, en_peasant) & enemy;
}

bool is_move_legal(const Board &board, i32 start, i32 end, Piece piece, Side side, i32 en_peasant) {
    u64 moves = get_moves(board, start, piece, side, en_peasant);
    u64 cand = 1ull << end;

    // is_check(const Board &board)

    return moves & cand;
}

bool is_check(const Board &board, i32 square, Side side, i32 en_peasant) {

    // following can be used to find i32 square given bitboard:
    // i32 sq = __builtin_ctzl(u64 bit_board);
    
    // bad naming(?)
    // should new board be created to evaluate check??

    u64 s = get_captures(board, square, R, side, en_peasant);
    u64 d = get_captures(board, square, B, side, en_peasant);
    u64 h = get_captures(board, square, N, side, en_peasant);
    u64 p = get_captures(board, square, P, side, en_peasant);

    u64 r_att = s & (board.pieces[R] | board.pieces[Q]);
    u64 d_att = d & (board.pieces[B] | board.pieces[B]);
    u64 h_att = h & board.pieces[N];
    u64 p_att = p & board.pieces[P];

    return r_att | d_att | h_att | p_att;
}

Board make_move(const Board &board, i32 start, i32 end, Piece piece, Side side, i32 en_peasant) {
    Board new_board = board;

    u64 s_mask = ~(1ull << start); // &
    u64 e_mask = 1ull << end; // |

    // shitty naming
    new_board.pieces[piece] &= s_mask;
    new_board.pieces[piece] |= e_mask;

    new_board.sides[side] &= s_mask;
    new_board.sides[side] |= e_mask;
    new_board.sides[side ^ 1] &= ~e_mask;

    return new_board;
}

void exec_move(i32 start, i32 end, Piece piece, Side side) {

}

// void exec_castle(bool kingside, u8 side) {
//     if (!side) {
//         if (kingside) {
//             white_pieces ^= ((u64) 0xf0);
//             kings ^= ((u64) 0x50);
//             rooks ^= ((u64) 0xa0);
//             // cleanliness
//             pieces[4] = 0;
//             pieces[5] = wR;
//             pieces[6] = wK;
//             pieces[7] = 0;
//         } else {
//             white_pieces ^= ((u64) 0x1d);
//             kings ^= ((u64) 0x14);
//             rooks ^= ((u64) 0x09);
//             pieces[0] = 0;
//             pieces[2] = wK;
//             pieces[3] = wR;
//             pieces[4] = 0;
//         }
//     } else {
//         if (kingside) {
//             black_pieces ^= 0xf000000000000000;
//             kings ^= 0x5000000000000000;
//             rooks ^= 0xa000000000000000;
//             pieces[4 + 56] = 0;
//             pieces[5 + 56] = bR;
//             pieces[6 + 56] = bK;
//             pieces[7 + 56] = 0;
//         } else {
//             black_pieces ^= 0x1d00000000000000;
//             kings ^= 0x1400000000000000;
//             rooks ^= 0x0900000000000000;
//             pieces[0 + 56] = 0;
//             pieces[2 + 56] = bK;
//             pieces[3 + 56] = bR;
//             pieces[4 + 56] = 0;
//         }
//     }
// }

// void exec_en_pessant(i32 start, i32 end, u8 side) {
    
// }

// // TODO: DO NOT ALSO DO A PAWN PUSH, CHANGE!!!
// void exec_promotion(i32 start, Piece promote_to, u8 side) {
//     if (!side) {
//         white_pieces ^= (0x0101000000000000 << COL(start));
//         pawns &= ~(0x0001000000000000 << COL(start));
//         board[promote_to] |= (0x0100000000000000 << COL(start));
//         pieces[start] = 0;
//         pieces[start + 8] = promote_to + 1;
//     } else {
//         black_pieces ^= ((u64) 0x0101 << COL(start));
//         pawns &= ~(0x0000000000000100 << COL(start));
//         board[promote_to] |= (0x0000000000000001 << COL(start));
//         pieces[start] = 0;
//         pieces[start - 8] = promote_to + 6;
//     }
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
