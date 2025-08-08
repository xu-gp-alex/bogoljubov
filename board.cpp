#include "protos.hpp"

// delete me
#include <iostream>

/* helper functions */
// todo: reorder to ts...
// todo: replace the fucking devisions with the nicer row and file bitwise solns

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
// bool make_move(move curr) {
//     pieces[curr.to] = pieces[curr.from];
//     pieces[curr.from] = 0;
//     ply++;
    
//     // notes: works?
//     side = ~side;
//     return true;
// }

// todo: i lowkey hate working with structs for now?
// bool make_move(u64 from, u64 to) {
//     // u64 legal = get_legal_moves(from, white_pieces | black_pieces,  );
//     return false;
// }

void init_board() {
    for (int i = 0; i < 64; i++) {
        pieces[i] = init[i];
    }

    white_pieces = 0x000000000000ffff;
    // white_pieces = 0x00000000000000ff;
    black_pieces = 0xffff000000000000;
    // black_pieces = 0xff00000000000000;
    pawns = 0x00ff00000000ff00;
    // pawns = (u64) 0;
    // pawns = 0x00ff000000000000;
    rooks = 0x8100000000000081;
    knights = 0x4200000000000042;
    // knights = 0xffffffffffff0008;
    bishops = 0x2400000000000024;
    // bishops = 0xffffffffffff0008;
    queens = 0x0800000000000008;
    // queens = 0xffffffffffff0008;
    kings = 0x1000000000000010;

    ply = 0;
    side = 0;
}

// notes: function signature inspired by pradu kannan
// notes: proper data types for inputs (uint8_t??)
// todo: fucking inline??
u64 get_rook_moves(i32 square, u64 occupancy) {
    occupancy &= rook_masks[square];
    occupancy *= rook_magics[square];
    occupancy >>= (64 - 12);

    return rook_moves[square][occupancy];
}

// notes: is it a good idea for the function to take in side?
// todo: make this account for moves that expose the king to check
// todo: would be nice of these inputs are consts
//       (tho wouldn't matter? primitives alr passed by value anyways)
u64 get_rook_legal_moves(i32 square, u64 occupancy, u8 side) {
    // notes: fucking naming nameing nameing
    // notes: how tf to good engines handle side bruh
    u64 mask = (!side) ? ~white_pieces : ~black_pieces;
    return get_rook_moves(square, occupancy) & mask;
}

u64 get_rook_captures(i32 square, u64 occupancy, u8 side) {
    u64 mask = (!side) ? black_pieces : white_pieces;
    return get_rook_legal_moves(square, occupancy, side) & mask;
}

u64 get_bishop_moves(i32 square, u64 occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magics[square];
    occupancy >>= (64 - 9);

    return bishop_moves[square][occupancy];
}

// notes: a good method or not...
// notes: i hate the two switch cases
// notes: why do we pass in occupancy??
// notes: isn't it the case you don't want to see the wires?
// notes: seperate the sides?
u64 get_legal_moves(i32 square, u64 occupancy, i32 piece_type, u8 side) {
    u64 mask = (!side) ? ~white_pieces : ~black_pieces;

    switch (piece_type) {
        // case wP:
        // case bP:
        //     /* god have mercy on your soul */
        //     // cleanliness
        //     u64 captures = (!side) ? white_pawn_captures[square] & black_pieces :
        //             black_pawn_captures[square] & white_pieces;

        //     u64 pushes = 0;
        //     u64 evil_mask = ~(white_pieces | black_pieces);
        //     if (!side && square / 8 != 7) {
        //         pushes = (u64) 1 << (square + 8);
        //         pushes &= evil_mask;
        //     } else if (side && square / 8 != 0) {
        //         pushes = (u64) 1 << (square - 8);
        //         pushes &= evil_mask;
        //     }

        //     // add eligiblity for two spaces
        //     // | might be faster than + ??
        //     if (!pushes) {
        //         if (!side) {
        //             pushes |= (((u64) 1 << (square + 16)) & evil_mask);
        //         } else {
        //             pushes |= (((u64) 1 << (square - 16)) & evil_mask);
        //         }
        //     }

        //     return captures | pushes;

        case wN:
        case bN:
            return knight_masks[square] & mask;

        case wK:
        case bK:
            return king_masks[square] & mask;

        case wQ:
        case bQ:
            // notes: space/tab/newline this to look pretty...
            // printf("hiya\n");
            // debug_board(get_rook_moves(square, occupancy));
            // debug_board(mask);
            return (get_bishop_moves(square, occupancy) | 
                    get_rook_moves(square, occupancy)) & mask;

        case wB:
        case bB:
            return get_bishop_moves(square, occupancy) & mask;

        case wR:
        case bR:
            return get_rook_moves(square, occupancy) & mask;

        default:
            break;
    }

    if (piece_type == wP || piece_type == bP) {
        /* god have mercy on your soul */
        // cleanliness
        u64 captures = (!side) ? white_pawn_captures[square] & black_pieces :
                black_pawn_captures[square] & white_pieces;

        u64 pushes = 0;
        u64 evil_mask = ~(white_pieces | black_pieces);
        if (!side && square / 8 != 7) {
            pushes = (u64) 1 << (square + 8);
            pushes &= evil_mask;
        } else if (side && square / 8 != 0) {
            pushes = (u64) 1 << (square - 8);
            pushes &= evil_mask;
        }

        // add eligiblity for two spaces
        // | might be faster than + ??
        if ((!side && square / 8 == 1) || (side && square / 8 == 6)) {
            if (pushes) {
                if (!side) {
                    // printf("asdf\n");
                    pushes |= (((u64) 1 << (square + 16)) & evil_mask);
                } else {
                    pushes |= (((u64) 1 << (square - 16)) & evil_mask);
                }
            }
        }

        return captures | pushes;
    }

    // notes: ofc this should never happen
    return 0;
}

u64 get_captures(i32 square, u64 occupancy, i32 piece_type, u8 side) {
    u64 mask = (!side) ? black_pieces : white_pieces;
    return get_legal_moves(square, occupancy, piece_type, side) & mask;
}

void save_board() {
    prev_board[0] = white_pieces;
    prev_board[1] = black_pieces;
    prev_board[2] = pawns;
    prev_board[3] = rooks;
    prev_board[4] = knights;
    prev_board[5] = bishops;
    prev_board[6] = queens;
    prev_board[7] = kings;
}

void revert_board() {
    white_pieces = prev_board[0];
    black_pieces = prev_board[1];
    pawns = prev_board[2];
    rooks = prev_board[3];
    knights = prev_board[4];
    bishops = prev_board[5];
    queens = prev_board[6];
    kings = prev_board[7];
}

/**
 * returns true if move is legal
 * side effect: also executes move lol
 */
bool accept_move(i32 start, i32 end, i32 piece_type, u64 occupancy, u8 side) {
    // ?? 

    u64 true_start = (u64) 1 << start;
    u64 goofy_ahh = (!side) ? white_pieces : black_pieces;
    if (!(true_start & goofy_ahh)) {
        printf("wrong side\n");
        return false;
    }

    u64 true_end = (u64) 1 << end;
    if (!(true_end & get_legal_moves(start, occupancy, piece_type, side))) {
        // debug_board(get_legal_moves(start, occupancy, piece_type, side));
        printf("piece cannot make such move\n");
        return false;
    }

    save_board();
    exec_move(start, end, piece_type, side);
    if (is_check(side, white_pieces | black_pieces)) {
        revert_board();
        return false;
    }

    pieces[start] = 0;
    pieces[end] = piece_type;

    // exec_move()
    return true;
}

// bool is_legal(i32 start, i32 end, i32 piece_type, u64 occupancy, u8 side) {
//     u64 s_mask = ~((u64) 1 << start);
//     u64 e_mask = (u64) 1 << start;

//     u64 temp_occ = occupancy;
//     temp_occ & s_mask;
//     temp_occ | e_mask;

//     save_board();
//     exec_move(start, end, piece_type, side);
//     bool res = is_check(side, temp_occ);


//     return res;
// }

bool is_check(u8 side, u64 occupancy) {

    // if (side == 0) {
    //     side = 1;
    // } else {
    //     side = 0;
    // }

    u64 fuck = (side) ? white_pieces : black_pieces;

    // i32 square = fuck & kings;
    u64 square = (!side) ? white_pieces & kings : black_pieces & kings;
    // debug_board(square);
    i32 sq = __builtin_ctzl(square);
    

    u64 straight = get_legal_moves(sq, occupancy, wR, side);
    u64 diagonal = get_legal_moves(sq, occupancy, wB, side);
    u64 horsey = get_legal_moves(sq, occupancy, wN, side);
    // debug_board(straight);
    // debug_board(diagonal);
    // debug_board(horsey);

    u64 s = straight & (rooks | queens) & fuck;
    u64 d = diagonal & (bishops | queens) & fuck;
    u64 h = horsey & knights & fuck;

    return s | d | h;
}

void exec_move(i32 start, i32 end, i32 piece_type, u8 side) {
    u64 s_mask = ~((u64) 1 << start); // &
    u64 e_mask = (u64) 1 << end; // |

    switch (piece_type) {
        case wP:
        case bP:
            pawns &= s_mask;
            pawns |= e_mask;
            break;

        case wN:
        case bN:
            knights &= s_mask;
            knights |= e_mask;
            break;

        case wK:
        case bK:
            kings &= s_mask;
            kings |= e_mask;
            break;

        case wQ:
        case bQ:
            queens &= s_mask;
            queens |= e_mask;
            break;

        case wB:
        case bB:
            bishops &= s_mask;
            bishops |= e_mask;
            break;

        case wR:
        case bR:
            rooks &= s_mask;
            rooks |= e_mask;
            break;

        default:
            break;
    }
    
    if (!side) {
        white_pieces &= s_mask;
        white_pieces |= e_mask;

        black_pieces &= ~e_mask;
    } else {
        black_pieces &= s_mask;
        black_pieces |= e_mask;

        white_pieces &= ~e_mask;
    }
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
