#include <iostream>
#include <map>

#include "cli.hpp"

// notes: having this const breaks [] operator for some reason
std::map<uint8_t, std::string> itoa = {
    {  0, " "}, 
    { wP, "♟"}, 
    { wN, "♞"}, 
    { wK, "♚"}, 
    { wQ, "♛"}, 
    { wB, "♝"}, 
    { wR, "♜"}, 
    { bP, "♙"}, 
    { bN, "♘"}, 
    { bK, "♔"}, 
    { bQ, "♕"}, 
    { bB, "♗"}, 
    { bR, "♖"}, 
};

// todo: (IMPORTANT) make a more robust way in general to test the functions
// todo: fucking learn to debug beyond debug statements kms kms kms
std::string rev_str[] = {
    "0000", "1000", "0100", "1100", 
    "0010", "1010", "0110", "1110", 
    "0001", "1001", "0101", "1101", 
    "0011", "1011", "0111", "1111",
};

// todo: somehow clear terminal, so it looks like stationary chess board
// todo: also allow quick refresh so it looks like timer is counting down
// todo: is printing out the board inefficient? also does it matter if it is?
void print_pieces(const u8 (&pieces)[64]) {
    std::cout << "+---+---+---+---+---+---+---+---+\n";
    for (int i = 7; i >= 0; i--) {
        std::cout<< "| ";
        for (int j = 0; j < 8; j++) {
            std::cout << itoa[pieces[i * 8 + j]] << " | ";
        }
        std::cout << "\n+---+---+---+---+---+---+---+---+\n";
    }
}

void print_bitboards(const Board &board) {
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
            u64 curr = board.pieces[j];
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

void debug_bitboard(u64 curr) {
    for (int i = 15; i >= 0; i-=2) {
        std::cout << rev_str[(curr >> (4 * (i-1))) & 0xF];
        std::cout << rev_str[(curr >> (4 * i)) & 0xF];
        std::cout << '\n';
    }
    std::cout << '\n';
}

void debug_bitboard(u64 curr, std::string label) {
    std::cout << label << '\n';
    debug_bitboard(curr);
}

std::map<char, Piece> str_to_piece = {
    {'N', N},
    {'Q', Q},
    {'B', B},
    {'R', R}
};

bool valid_str(std::string input) {
    if (input.length() != 4 && input.length() != 6) {
        return false;
    }

    if (input[0] < 'a' || input[0] > 'h' || input[2] < 'a' || input[2] > 'h') {
        return false;
    }

    if (input[1] < '1' || input[1] > '8' || input[3] < '1' || input[3] > '8') {
        return false;
    }

    if (input.length() == 6 && (input[4] != '=' || !str_to_piece.count(input[5]))) {
        return false;
    }

    return true;
}

move str_to_move(std::string input, const u8 (&pieces)[64]) {
    move res;

    res.start = (input[1] - '1') * 8 + (input[0] - 'a');
    res.end = (input[3] - '1') * 8 + (input[2] - 'a');
    res.promote = (input.length() == 6) ? str_to_piece[input[5]] : X;

    // backwards compatibility :skull:
    switch (pieces[res.start]) {
        case wP:
        case bP:
            res.piece = P;
            break;

        case wN:
        case bN:
            res.piece = N;
            break;

        case wK:
        case bK:
            res.piece = K;
            break;

        case wQ:
        case bQ:
            res.piece = Q;
            break;

        case wB:
        case bB:
            res.piece = B;
            break;

        case wR:
        case bR:
            res.piece = R;
            break;

        default:
            res.piece = X;
            break;
    }

    return res;
}