#include "cli.hpp"

#include <iostream>
#include <map>

// always add one i guess
const std::string w_piece_to_str[] = {" ", "♟", "♞", "♚", "♛", "♝", "♜"};
const std::string b_piece_to_str[] = {" ", "♙", "♘", "♔", "♕", "♗", "♖"};

// todo: is printing out the board inefficient? also does it matter if it is?
void print_pieces(const Board &board) {
    std::cout << "+---+---+---+---+---+---+---+---+\n";
    for (int i = 7; i >= 0; i--) {
        std::cout<< "| ";
        for (int j = 0; j < 8; j++) {
            i32 curr_piece = board.pieces[i * 8 + j] + 1;
            bool is_white = board.sides[White] & (1ull << (i * 8 + j));

            if (curr_piece == X || is_white) {
                std::cout << w_piece_to_str[curr_piece] << " | ";
            } else {
                std::cout << b_piece_to_str[curr_piece] << " | ";
            }
        }
        std::cout << "\n+---+---+---+---+---+---+---+---+\n";
    }
}

void print_move(const Move &move) {
    if (move.start == null_move.start) return;

    std::cout << (char) (97 + (move.start & 7));
    std::cout << (move.start >> 3) + 1;
    std::cout << (char) (97 + (move.end & 7));
    std::cout << (move.end >> 3) + 1;

    std::cout << '\n';
}

void debug_pieces(const Board &board) {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            i32 curr_piece = board.pieces[i * 8 + j] + 1;
            std::cout << w_piece_to_str[curr_piece];
        }
        std::cout << '\n';
    }
}

// todo: (IMPORTANT) make a more robust way in general to test the functions
// todo: fucking learn to debug beyond debug statements kms kms kms
std::string rev_str[] = {
    "0000", "1000", "0100", "1100", 
    "0010", "1010", "0110", "1110", 
    "0001", "1001", "0101", "1101", 
    "0011", "1011", "0111", "1111",
};

void debug_bbs(const Board &board) {
    std::cout << '\n';
    std::cout << "P:        ";
    std::cout << "N:        ";
    std::cout << "K:        ";
    std::cout << "Q:        ";
    std::cout << "B:        ";
    std::cout << "R:        ";
    std::cout << "White:    ";
    std::cout << "Black:    ";
    std::cout << '\n';

    for (int i = 15; i >= 0; i-=2) {
        for (int j = 0; j < 6; j++) {
            u64 curr = board.pieces_bb[j];
            std::cout << rev_str[(curr >> (4 * (i-1))) & 0xF];
            std::cout << rev_str[(curr >> (4 * i)) & 0xF];
            std::cout << "  ";
        }

        for (int j = 1; j >= 0; j--) {
            u64 curr = board.sides[j]; 
            std::cout << rev_str[(curr >> (4 * (i-1))) & 0xF];
            std::cout << rev_str[(curr >> (4 * i)) & 0xF];
            std::cout << "  ";
        }

        std::cout << '\n';
    }
    std::cout << '\n';
}

void debug_board(const Board &board) {
    std::cout<< "pieces: \n";
    debug_pieces(board);
    debug_bbs(board);
    std::cout << "m2s: " << board.m2s << '\n';
    std::cout << "wKc: " << board.can_white_k_castle << '\n';
    std::cout << "wQc: " << board.can_white_q_castle << '\n';
    std::cout << "bKc: " << board.can_black_k_castle << '\n';
    std::cout << "bQc: " << board.can_black_q_castle << '\n';
}

void debug_bb(u64 curr) {
    for (int i = 15; i >= 0; i-=2) {
        std::cout << rev_str[(curr >> (4 * (i-1))) & 0xF];
        std::cout << rev_str[(curr >> (4 * i)) & 0xF];
        std::cout << '\n';
    }
    std::cout << '\n';
}

void debug_bb(u64 curr, std::string label) {
    std::cout << label << '\n';
    debug_bb(curr);
}

std::map<char, Piece> valid_promotions = {
    {'n', N},
    {'q', Q},
    {'n', B},
    {'r', R}
};

bool valid_str(std::string input) {
    if (input.length() != 4 && input.length() != 5) {
        return false;
    }

    if (input[0] < 'a' || input[0] > 'h' || input[2] < 'a' || input[2] > 'h') {
        return false;
    }

    if (input[1] < '1' || input[1] > '8' || input[3] < '1' || input[3] > '8') {
        return false;
    }

    if (input.length() == 5 && !valid_promotions.count(input[4])) {
        return false;
    }

    return true;
}

// lowk in board.cpp?
Move str_to_move(const Board &board, std::string input) {
    Move res = null_move;

    res.start = (input[1] - '1') * 8 + (input[0] - 'a');
    res.end = (input[3] - '1') * 8 + (input[2] - 'a');

    if (input.length() == 5) {
        res.promote = valid_promotions[input[4]];
    }

    Piece piece = board.pieces[res.start];
    Piece captured = board.pieces[res.end];

    if (piece == K && res.end - res.start == 2) {
        res.k_castle = true;
    }

    if (piece == K && res.start - res.end == 2) {
        res.q_castle = true;
    }

    // will this lead to excessive checks for en peasant?
    // jank: check capture by seeing files don't align
    if (piece == P && (res.start & 7) != (res.end & 7) && captured == X) {
        res.en_peasant = true;
    }

    return res;
}

