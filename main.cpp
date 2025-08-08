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
        // debug_board(pawns);
        // debug_board(black_pieces);
        // debug_board(get_legal_moves(59, white_pieces | black_pieces, bQ, 1));
        // debug_board(get_legal_moves(3, white_pieces | black_pieces, wQ, 0));
        // debug_board(rook_masks[59]);
        // debug_board(bishop_masks[59]);
        // debug_board((u64) 0xf7ff);
        // debug_board(get_legal_moves(3, (u64) 0xf7ff, wQ, 0));
        print_board();
    }
    return 0;
}