#include <iostream>
#include <string>
#include <map>

#include "protos.hpp"
#include "cli.hpp"

int main() {
    init_board();
    std::cout << "loading magics...\n";
    generate_rook_magics();
    generate_bishop_magics();

    // cli_game_loop()
    std::cout << "\nWelcome to BOGOLJUBOV chess engine :wilted-rose:\n";
    std::cout << "- enter \"q\" to quit\n";
    // std::cout << "- enter \"r\" to reset\n\n";

    Board board = get_new_board();
    print_bitboards(board);

    for (;;) {
        std::cout << "User Move: ";
        std::string input;
        std::cin >> input;
        if (input == "q") {
            break;
        } else if (input == "r") {
            init_board();
            board = get_new_board();
            print_bitboards(board);
            continue;
        }

        if (valid_str(input)) {
            move u = str_to_move(input, pieces);

            // testing
            if (side) {
                debug_bitboard(get_white_pawn_moves(board, u.start, en_peasant), "white-pawn");
            } else {
                debug_bitboard(get_black_pawn_moves(board, u.start, en_peasant), "black-pawn");
            }
            
            if (is_move_legal(board, u.start, u.end, u.piece, side, en_peasant)) {
                board = make_move(board, u.start, u.end, u.piece, side, en_peasant);

                side = (side) ? Black : White;
            } else {
                std::cout << "invalid move, try again\n";
            }
        }

        print_bitboards(board);
    }

    return 0;
}