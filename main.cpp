#include <string>

#include "protos.hpp"
#include "cli.hpp"

// testing
#include <iostream>

int main() {
    init_board();
    std::cout << "loading magics...\n";
    generate_rook_magics();
    generate_bishop_magics();

    // cli_game_loop()
    std::cout << "\nWelcome to BOGOLJUBOV chess engine :wilted-rose:\n";
    std::cout << "- enter \"q\" to quit\n\n";

    // std::cout << "current occupancy\n";
    // for (int i = 0; i < 7; i++) {
    //     debug_board(get_legal_moves(i, white_pieces | black_pieces, pieces[i], 0));
    // }

    // debug_board(white_pieces);
    // debug_board(black_pieces);
    // debug_board(pawns);
    // debug_board(rooks);
    // debug_board(knights);
    // debug_board(bishops);
    // debug_board(queens);
    // debug_board(kings);

    // if (is_check(0, white_pieces | black_pieces)) {
    //     std::cout << "in check\n";
    // } else {
    //     std::cout << "safe\n";
    // }

    // debug_board(get_knight_moves(1, white_pieces | black_pieces));
    print_board();
    for (;;) {
        std::cout << "User Move: ";
        std::string despair;
        std::cin >> despair;
        if (despair == "q") {
            break;
        }
        // } else if (despair == "p") {
        //     print_board();
        //     continue;
        // }

        bool res = user_move(despair);
        if (!res) {
            printf("invalid move, please try again\n");
        } else {
            side = (!side) ? 1 : 0; 
            // debug_board(white_pieces);
            // debug_board(black_pieces);
        }
        print_board();
    }
    return 0;
}