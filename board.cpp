#include <cstdlib>
#include <iostream> // deleteme

#include "protos.hpp"

/* helper functions */
// todo: reorder ts...

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
    // for (int i = 0; i < 64; i++) {
    //     pieces[i] = init[i];
    // }

    ply = 0;
    side = White;

    en_peasant = -1;

    can_white_k_castle = true;
    can_white_q_castle = true;
    can_black_k_castle = true;
    can_black_q_castle = true;
}

// reference instead?? kms
Board get_new_board() {
    Board new_board;

    for (int i = 0; i < 64; i++) {
        new_board.pieces[i] = init[i];
    }

    new_board.pieces_bb[P] = 0x00ff00000000ff00ull;
    new_board.pieces_bb[N] = 0x4200000000000042ull;
    new_board.pieces_bb[K] = 0x1000000000000010ull;
    new_board.pieces_bb[Q] = 0x0800000000000008ull;
    new_board.pieces_bb[B] = 0x2400000000000024ull;
    new_board.pieces_bb[R] = 0x8100000000000081ull;

    new_board.sides[Black] = 0xffff000000000000ull;
    new_board.sides[White] = 0x000000000000ffffull;
    return new_board;
}

// notes: proper data types for inputs (uint8_t??)
// todo: fucking inline??

u64 get_white_pawn_pushes(i32 square, u64 occupancy) {
    u64 pushes = (1ull << (square + 8)) & ~occupancy;
    if (ROW(square) == 1 && pushes) {
        pushes |= (1ull << (square + 16)) & ~occupancy;
    }
    return pushes;
}

u64 get_white_pawn_captures(i32 square) {
    return white_pawn_captures[square];
}

u64 get_black_pawn_pushes(i32 square, u64 occupancy) {
    u64 pushes = (1ull << (square - 8)) & ~occupancy;
    if (ROW(square) == 6 && pushes) {
        pushes |= (1ull << (square - 16)) & ~occupancy;
    }

    return pushes;
}

u64 get_black_pawn_captures(i32 square) {
    return black_pawn_captures[square];
}

u64 get_knight_moves(i32 square) {
    return knight_masks[square];
}

u64 get_king_moves(i32 square) {
    return king_masks[square];
}

u64 get_bishop_moves(i32 square, u64 occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magics[square];
    occupancy >>= (64 - 9);

    return bishop_moves[square][occupancy];
}

/**
 * (function signature inspired by pradu kannan)
 */
u64 get_rook_moves(i32 square, u64 occupancy) { 
    occupancy &= rook_masks[square];
    occupancy *= rook_magics[square];
    occupancy >>= (64 - 12);

    return rook_moves[square][occupancy];
}

u64 get_queen_moves(i32 square, u64 occupancy) {
    return get_rook_moves(square, occupancy) | get_bishop_moves(square, occupancy);
}

/**
 * (jank function)
 * add 1 to bitboard if no pieces in way of kingside castle
 */
u64 case_k_castle(i32 square, u64 occupancy) {
    u64 res = 1ull << (square + 2);
    occupancy &= 3ull << (square + 1);
    return (!occupancy) ? res : 0ull;
}

/**
 * (jank function)
 * add 1 to bitboard if no pieces in way of queenside castle
 */
u64 case_q_castle(i32 square, u64 occupancy) {
    u64 res = 1ull << (square - 2);
    occupancy &= 7ull << (square - 3);
    return (!occupancy) ? res : 0ull;
}

// not implemented
u64 case_en_passant(i32 square, u64 pawns, i32 en_peasant) {
    return 0ull;
}

/**
 * gets pseudolegal moves in an evil manner.
 * case_k_castle literally makes no fucking sense.
 * en passant is fucking impossible to test.
 * also shitty parameter names as usual.
 * @param en_peasant file of pawn which recently moved 2 spaces (`-1` if not applicable)
 * @param k_castle has kingside castle occurred yet?
 * @param q_castle has queenside castle occurred yet?
 */
u64 get_moves(const Board &board, i32 square, i32 en_peasant, bool k_castle, bool q_castle, Side side) {
    u64 occupancy = board.sides[Black] | board.sides[White];
    u64 friendly = board.sides[side];
    // what in the fuck is this shit kms kms kms
    u64 enemy = board.sides[side ^ 1];
    Piece piece = board.pieces[square];

    u64 res = 0ull;
    switch (piece) {
        case P:
            if (side) {
                // ugliest fucking expression
                if (en_peasant != -1) enemy |= (1ull << (en_peasant + 40));
                res = (get_white_pawn_pushes(square, occupancy) & ~friendly) | 
                        (get_white_pawn_captures(square) & enemy);
            } else {
                if (en_peasant != -1) enemy |= (1ull << (en_peasant + 16));
                res = (get_black_pawn_pushes(square, occupancy) & ~friendly) | 
                        (get_black_pawn_captures(square) & enemy);
            }
            break;

        case N:
            res = get_knight_moves(square) & ~friendly;
            break;

        case K:
            if (k_castle) res |= case_k_castle(square, occupancy);
            if (q_castle) res |= case_q_castle(square, occupancy);
            res |= (get_king_moves(square) & ~friendly);
            break;

        case Q:
            res = get_queen_moves(square, occupancy) & ~friendly;
            break;

        case B:
            res = get_bishop_moves(square, occupancy) & ~friendly;
            break;

        case R:
            res = get_rook_moves(square, occupancy) & ~friendly;
            break;

        default:
            break;
    }

    return res;
}

bool is_move_legal(const Board &board, i32 start, i32 end, i32 en_peasant, bool k_castle, bool q_castle, Side side, Piece promotion) {
    // retarded bug where the king just changes sides; what the fuck
    u64 pieces_which_can_fucking_move = board.sides[side];
    if (!((1ull << start) & pieces_which_can_fucking_move)) {
        return false;
    }

    // there exists better tway to do ts
    if (promotion != X) {
        if (board.pieces[start] != P || (ROW(end) != 0 && ROW(end) != 7)) {
            return false;
        }
    }

    u64 moves = get_moves(board, start, en_peasant, k_castle, q_castle, side);
    u64 cand = 1ull << end;

    return moves & cand;
}

// horrendous naming / argument placement
// really don't want to pass in Side side

/**
 * @brief rename these fucking parameters:
 * @param s_p enemy horizontal or vertical attackers
 * @param d_p enemy diagonal attackers
 * @param h_p enemy knights
 * @param p_p enemy pawns
 * @return
 */
bool is_check(
    i32 square, 
    u64 occupancy, 
    u64 s_p, 
    u64 d_p, 
    u64 h_p, 
    u64 p_p, 
    Side side
) {

    // following can be used to find i32 square given bitboard:
    // i32 sq = __builtin_ctzl(u64 bit_board);

    u64 s = get_rook_moves(square, occupancy);
    u64 d = get_bishop_moves(square, occupancy);
    u64 h = get_knight_moves(square);

    u64 p = (side) ? get_white_pawn_captures(square) : get_black_pawn_captures(square);

    u64 s_att = s & s_p;
    u64 d_att = d & d_p;
    u64 h_att = h & h_p;
    u64 p_att = p & p_p;

    return s_att | d_att | h_att | p_att;
}

Board exec_en_passant(const Board &board, i32 start, i32 end, i32 en_peasant) {
    Board new_board = board;

    u64 death_of_joy = 1ull << start;
    u64 hopelessness = 1ull << end;
    u64 aimlessness = 1ull << (ROW(start) * 8 + en_peasant);

    new_board.sides[side] ^= (death_of_joy | hopelessness);
    new_board.sides[side ^ 1] ^= aimlessness;

    new_board.pieces_bb[P] ^= (death_of_joy | hopelessness | aimlessness);
    
    new_board.pieces[start] = X;
    new_board.pieces[end] = P;
    new_board.pieces[ROW(start) * 8 + en_peasant] = X;

    return new_board;
}

Board exec_k_castle(const Board &board, i32 start) {
    Board new_board = board;

    u64 king_mask = 0x5ull << start;
    u64 rook_mask = 0xaull << start;

    new_board.sides[side] ^= (king_mask | rook_mask);

    new_board.pieces_bb[K] ^= king_mask;
    new_board.pieces_bb[R] ^= rook_mask;
    
    new_board.pieces[start] = X;
    new_board.pieces[start + 1] = R;
    new_board.pieces[start + 2] = K;
    new_board.pieces[start + 3] = X;

    return new_board;
}

Board exec_q_castle(const Board &board, i32 start) {
    Board new_board = board;

    u64 king_mask = 0x14ull << (start - 4);
    u64 rook_mask = 0x9ull << (start - 4);

    new_board.sides[side] ^= (king_mask | rook_mask);

    new_board.pieces_bb[K] ^= king_mask;
    new_board.pieces_bb[R] ^= rook_mask;
    
    new_board.pieces[start] = X;
    new_board.pieces[start - 1] = R;
    new_board.pieces[start - 2] = K;
    new_board.pieces[start - 4] = X;

    return new_board;
}

Board exec_promotion(const Board &board, i32 start, i32 end, Piece promotion) {
    Board new_board = board;

    u64 s_mask = 1ull < start;
    u64 e_mask = 1ull < end;

    new_board.sides[side] ^= (s_mask | e_mask);

    new_board.pieces_bb[P] &= ~s_mask;
    new_board.pieces_bb[promotion] |= e_mask;

    new_board.pieces[start] = X;
    new_board.pieces[end] = promotion;

    return new_board;
}

/**
 * should recognize castles
 * should recognize en passant
 * search will iterate through u64 get_moves()
 * if start and end as sus enough, make_move
 * should return the correct, following position
 * ASSUME ALL MOVES ARE LEGAL
 */
Board make_move(const Board &board, i32 start, i32 end, i32 en_peasant, bool k_castle, bool q_castle, Side side, Piece promotion) {
    // 8-16-25 8:45pm, i hate everything
    // bad variables..
    if (board.pieces[start] == P && COL(start) != COL(end) && board.pieces[end] == X) return exec_en_passant(board, start, end, en_peasant);
    if (board.pieces[start] == K && end - start == 2) return exec_k_castle(board, start);
    if (board.pieces[start] == K && start - end == 2) return exec_q_castle(board, start);
    if (promotion != X) exec_promotion(board, start, end, promotion);

    Board new_board = board;

    Piece piece = board.pieces[start];
    u64 s_mask = ~(1ull << start); // &
    u64 e_mask = 1ull << end; // |

    // shitty naming
    new_board.pieces_bb[piece] &= s_mask;
    new_board.pieces_bb[piece] |= e_mask;

    new_board.sides[side] &= s_mask;
    new_board.sides[side] |= e_mask;
    new_board.sides[side ^ 1] &= ~e_mask;

    new_board.pieces[start] = X;
    new_board.pieces[end] = piece;

    return new_board;
}

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
