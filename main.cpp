#include <iostream>
#include <string>
#include <map>

#include "protos.hpp"

std::map<u8, std::string> itoa = {
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

// todo: also allow quick refresh so it looks like timer is counting down
// todo: is printing out the board inefficient? also does it matter if it is?
void print_board() {
    std::cout << "+---+---+---+---+---+---+---+---+\n";
    for (int i = 7; i >= 0; i--) {
        std::cout<< "| ";
        for (int j = 0; j < 8; j++) {
            std::cout << itoa[pieces[i * 8 + j]] << " | ";
        }
        std::cout << "\n+---+---+---+---+---+---+---+---+\n";
    }
}

std::map<char, Piece> str_to_piece = {
    {'N', N},
    {'Q', Q},
    {'B', B},
    {'R', R}
};

// notes: castling is just the king moving two spaces sometimes lmao
// should return move struct, function doing too much(?)
bool user_move(std::string curr) {
    if (curr.length() != 4 && curr.length() != 6) {
        return false;
    }

    if (curr[0] < 'a' || curr[0] > 'h' || curr[2] < 'a' || curr[2] > 'h') {
        return false;
    }

    if (curr[1] < '1' || curr[1] > '8' || curr[3] < '1' || curr[3] > '8') {
        return false;
    }

    if (curr.length() == 6 && (curr[4] != '=' || !str_to_piece.count(curr[5]))) {
        return false;
    }

    i32 start = (curr[1] - '1') * 8 + (curr[0] - 'a');
    i32 end = (curr[3] - '1') * 8 + (curr[2] - 'a');
    i32 piece_type = pieces[start];
    Piece promote = (curr.length() == 6) ? str_to_piece[curr[5]] : X;

    // check here?
    if (piece_type == 0) {
        printf("no piece selected\n");
        return false;
    }

    // bool res = make_move(start, end, piece_type, white_pieces | black_pieces, side);
    // return res;
    return false;
}

int main() {
    init_board();
    std::cout << "loading magics...\n";
    generate_rook_magics();
    generate_bishop_magics();

    // cli_game_loop()
    std::cout << "\nWelcome to BOGOLJUBOV chess engine :wilted-rose:\n";
    std::cout << "- enter \"q\" to quit\n";
    std::cout << "- enter \"r\" to restart\n\n";

    return 0;
    for (;;) {
        std::cout << "User Move: ";
        std::string despair;
        std::cin >> despair;
        if (despair == "q") {
            break;
        } else if (despair == "r") {
            init_board();
            print_board();
            continue;
        }

        bool res = user_move(despair);
        if (!res) {
            printf("invalid move, please try again\n");
        } else {
            if (!side) {
                black_en_pessant_tgt = -1;
            } else {
                white_en_pessant_tgt = -1;
            }

            printf("white_tgt: %d\n", white_en_pessant_tgt);
            printf("black_tgt: %d\n", black_en_pessant_tgt);

            side = (!side) ? 1 : 0; 
        }
        
        print_board();
    }
    return 0;
}