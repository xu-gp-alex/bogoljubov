#include "protos.hpp"
// #include "rook_moves.hpp"
// #include "bishop_moves.hpp"

#include <cstdlib>
#include <string>
#include <iostream> // deleteme

// (refer to cpw flipping bits)
// (horrible no good reference)

/**
 * flips the n-th bit of the value `bits` to 1
 * note this **directly modifies** `bits`
 */
void flip_to_1(u64 &bits, i32 n) { // flip_to_1 --> set
    bits |= (1ull << n);
}

/**
 * flips the n-th bit of the value `bits` to 0
 * note this **directly modifies** `bits`
 */
void flip_to_0(u64 &bits, i32 n) { // flip_to_0 --> clear
    bits &= ~(1ull << n);
}

/* toggle? */

/* helper functions */

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

void init_data() {
    ply = 0;
    side = White;
    decisive_result = false;
}

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

    new_board.m2s = -1;
    new_board.can_white_k_castle = true;
    new_board.can_white_q_castle = true;
    new_board.can_black_k_castle = true;
    new_board.can_black_q_castle = true;

    return new_board;
}

// not implemented
Board get_custom_board(std::string setup) {
    Board new_board;
    return new_board;
}

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
 * makes sure king is not kingside castling through pieces
 */
bool can_k_castle(i32 square, u64 occupancy) {
    occupancy &= 3ull << (square + 1);
    return occupancy == 0ull;
}

/**
 * makes sure king is not queenside castling through pieces
 */
bool can_q_castle(i32 square, u64 occupancy) {
    occupancy &= 7ull << (square - 3);
    return occupancy == 0ull;
}

/**
 * checks if the enemy pawn which moved two spaces
 * is in *file* next to current pawn
 */
bool can_en_peasant(i32 square, i32 enemy_m2s) {
    return abs(square - enemy_m2s) == 1;
}

/**
 * @brief returns bitboard representing all possible moves of a piece (on the specified square)
 * disregarding leaving or placing the king in check
 * @param m2s file of pawn which recently moved 2 spaces (`-1` if not applicable)
 * @param can_k_castle whether kingside castle is eventually possible
 * @param can_q_castle whether queenside castle is eventually possible
 * @return
 */
u64 get_pseudolegal_moves(u64 friendly, u64 enemy, i32 square, Piece piece, bool k_castle_possible, bool q_castle_possible, i32 enemy_m2s) {
    u64 occupancy = friendly | enemy;
    u64 moves = 0ull;

    if (piece == P) {
        if (side) {
            moves = get_white_pawn_pushes(square, occupancy);
            moves |= get_white_pawn_captures(square) & enemy;
        } else {
            moves = get_black_pawn_pushes(square, occupancy);
            moves |= get_black_pawn_captures(square) & enemy;
        }

        // not sufficient to show en_passant is possible
        if (enemy_m2s != -1 && can_en_peasant(square, enemy_m2s)) {
            if (side) {
                flip_to_1(moves, enemy_m2s + 8);
            } else {
                flip_to_1(moves, enemy_m2s - 8);
            }
        }

    } else if (piece == N) {
        moves = get_knight_moves(square) & ~friendly;
    } else if (piece == K) {
        moves = get_king_moves(square) & ~friendly;

        if (k_castle_possible && can_k_castle(square, occupancy)) {
            flip_to_1(moves, square + 2);
        } 

        if (q_castle_possible && can_q_castle(square, occupancy)) {
            flip_to_1(moves, square - 2);
        }

    } else if (piece == Q) {
        moves = get_queen_moves(square, occupancy) & ~friendly;
    } else if (piece == B) {
        moves = get_bishop_moves(square, occupancy) & ~friendly;
    } else if (piece == R) {
        moves = get_rook_moves(square, occupancy) & ~friendly;
    }

    return moves;
}

// sadness
// why does this take side??
bool can_promote(i32 square, Side side) {
    return (side == White && ROW(square) == 6) ||
            (side == Black && ROW(square) == 1);
}

bool is_move_legal(const Board &board, Move m, Side side) {

    // temporary fix, now that `Board` stores some other stuff
    bool k_castle_possible = (side) ? board.can_white_k_castle : board.can_black_k_castle;
    bool q_castle_possible = (side) ? board.can_white_q_castle : board.can_black_q_castle;
    i32 enemy_m2s = board.m2s;

    u64 friendly = board.sides[side];
    u64 enemy = board.sides[side ^ 1];
    u64 piece = board.pieces[m.start];

    if ((friendly & (1ull << m.start)) == 0ull) return false; // can't move opponent's piece lol
    if (m.promote != X && !can_promote(m.start, side)) return false; // can't promote pawns before last rank
    if (m.promote == X && can_promote(m.start, side)) return false; // can't promote to pawn
    if (m.k_castle && !k_castle_possible) return false; // can't castle kingside
    if (m.q_castle && !q_castle_possible) return false; // can't castle queenside
    if (m.en_peasant && enemy_m2s == -1) return false; // nothing to en peasant

    u64 moves = get_pseudolegal_moves(friendly, enemy, m.start, board.pieces[m.start], k_castle_possible, q_castle_possible, enemy_m2s);
    u64 cand = 1ull << m.end;
    return moves & cand; // see if ending square is amongst the (pseudo)legal moves
}

bool is_check(i32 square, u64 occupancy, u64 enemy_R_or_Q, u64 enemy_B_or_Q, u64 enemy_N, u64 enemy_wP,u64 enemy_bP, Side side) {
    enemy_R_or_Q &= get_rook_moves(square, occupancy);
    enemy_B_or_Q &= get_bishop_moves(square, occupancy);
    enemy_N &= get_knight_moves(square);
    enemy_wP &= get_black_pawn_captures(square);
    enemy_bP &= get_white_pawn_captures(square);

    return enemy_R_or_Q | enemy_B_or_Q | enemy_N | enemy_wP | enemy_bP;
}

Board exec_en_passant(const Board &board, i32 start, i32 end) {
    Board new_board = board;

    u64 tgt_sq = (ROW(start) * 8) + COL(end);

    // update piece bitboards
    flip_to_0(new_board.pieces_bb[P], start);
    flip_to_1(new_board.pieces_bb[P], end);
    flip_to_0(new_board.pieces_bb[P], tgt_sq);

    // update color bitboards
    flip_to_0(new_board.sides[side], start);
    flip_to_1(new_board.sides[side], end);
    flip_to_0(new_board.sides[side ^ 1], tgt_sq);
    
    // update piece array
    new_board.pieces[start] = X;
    new_board.pieces[end] = P;
    new_board.pieces[tgt_sq] = X;

    // update states
    new_board.m2s = -1;

    return new_board;
}

Board exec_k_castle(const Board &board, i32 start, Side side) {
    Board new_board = board;

    u64 king_mask = 0x5ull << start;
    u64 rook_mask = 0xaull << start;

    // update piece bitboards
    new_board.pieces_bb[K] ^= king_mask;
    new_board.pieces_bb[R] ^= rook_mask;

    // update color bitboards
    new_board.sides[side] ^= (king_mask | rook_mask);
    
    // update piece array
    new_board.pieces[start] = X;
    new_board.pieces[start + 1] = R;
    new_board.pieces[start + 2] = K;
    new_board.pieces[start + 3] = X;

    // update states
    if (side) {
        new_board.can_white_k_castle = false;
        new_board.can_white_q_castle = false;
    } else {
        new_board.can_black_k_castle = false;
        new_board.can_black_q_castle = false;
    }
    new_board.m2s = -1;

    return new_board;
}

Board exec_q_castle(const Board &board, i32 start, Side side) {
    Board new_board = board;

    u64 king_mask = 0x14ull << (start - 4);
    u64 rook_mask = 0x9ull << (start - 4);

    // update piece bitboards
    new_board.pieces_bb[K] ^= king_mask;
    new_board.pieces_bb[R] ^= rook_mask;

    // update color bitboards
    new_board.sides[side] ^= (king_mask | rook_mask);
    
    // update piece array
    new_board.pieces[start] = X;
    new_board.pieces[start - 1] = R;
    new_board.pieces[start - 2] = K;
    new_board.pieces[start - 4] = X;

    // update states
    if (side) {
        new_board.can_white_k_castle = false;
        new_board.can_white_q_castle = false;
    } else {
        new_board.can_black_k_castle = false;
        new_board.can_black_q_castle = false;
    }
    new_board.m2s = -1;

    return new_board;
}

Board exec_promotion(const Board &board, i32 start, i32 end, Piece promotion, Piece captured, Side side) {
    Board new_board = board;

    // update piece bitboards
    flip_to_0(new_board.pieces_bb[P], start);
    flip_to_1(new_board.pieces_bb[promotion], end);
    if (captured != X) {
        flip_to_0(new_board.pieces_bb[captured], end);
    }

    // update color bitboards
    flip_to_0(new_board.sides[side], start);
    flip_to_1(new_board.sides[side], end);
    flip_to_0(new_board.sides[side ^ 1], end);

    // update piece array
    new_board.pieces[start] = X;
    new_board.pieces[end] = promotion;

    // update states
    new_board.m2s = -1;

    return new_board;
}

// ASSUME ALL MOVES ARE LEGAL
Board make_move(const Board &board, Move m, Side side) {
    if (m.promote != X) return exec_promotion(board, m.start, m.end, m.promote, board.pieces[m.end], side); // <-- jank as to how captured is passed
    if (m.k_castle) return exec_k_castle(board, m.start, side);
    if (m.q_castle) return exec_q_castle(board, m.start, side);
    if (m.en_peasant) return exec_en_passant(board, m.start, m.end); 

    Board new_board = board;
    Piece piece = board.pieces[m.start];
    Piece captured = board.pieces[m.end];

    // update piece bitboards
    flip_to_0(new_board.pieces_bb[piece], m.start);
    flip_to_1(new_board.pieces_bb[piece], m.end);
    if (captured != X) {
        flip_to_0(new_board.pieces_bb[captured], m.end);
    }

    // update color bitboards
    flip_to_0(new_board.sides[side], m.start);
    flip_to_1(new_board.sides[side], m.end);
    flip_to_0(new_board.sides[side ^ 1], m.end);

    // update piece array
    new_board.pieces[m.start] = X;
    new_board.pieces[m.end] = piece;

    // update states
    /* pawns pushed 2 squares */
    if (piece == P && abs(ROW(m.start) - ROW(m.end)) == 2) {
        new_board.m2s = m.end;
    } else {
        new_board.m2s = -1;
    }

    /* invalidating castling by moving rook */
    if (COL(m.start) == 0 && piece == R) {
        if (side) {
            new_board.can_white_q_castle = false;
        } else {
            new_board.can_black_q_castle = false;
        }
    }

    if (COL(m.start) == 7 && piece == R) {
        if (side) {
            new_board.can_white_k_castle = false;
        } else {
            new_board.can_black_k_castle = false;
        }
    }

    /* invalidating castling by moving king */
    if (piece == K) {
        if (side) {
            new_board.can_white_k_castle = false;
            new_board.can_white_q_castle = false;
        } else {
            new_board.can_black_k_castle = false;
            new_board.can_black_q_castle = false;
        }
    }

    return new_board;
}

/**
 * generates `move` struct
 * real purpose is to helps to set the promotion, castling, and en_peasant booleans
 */
Move get_move(i32 start, i32 end, Piece piece, Piece captured, Piece promote) {
    Move res = null_move;
    res.start = start;
    res.end = end;

    if (piece == K && end - start == 2) {
        res.k_castle = true;
    }

    if (piece == K && start - end == 2) {
        res.q_castle = true;
    }

    // will this lead to excessive checks for en peasant?
    // jank: check capture by seeing files don't align
    if (piece == P && COL(start) != COL(end) && captured == X) {
        res.en_peasant = true;
    }

    return res;
}

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
