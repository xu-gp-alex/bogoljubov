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
            
            if (is_move_legal(board, u.start, u.end, u.piece, side, en_peasant)) {
                board = make_move(board, u.start, u.end, u.piece, side, en_peasant);
                // make_move also takes castling rights? should the outside determine what boolean to pass in? (prolly yes)
                
                // pieces must be incorporated into Board struct first
                if (COL(u.start) == 0 && board.pieces[u.start] == R) {
                    if (side) {
                        can_white_q_castle = false;
                    } else {
                        can_black_q_castle = false;
                    }
                }

                if (COL(u.start) == 7 && board.pieces[u.start] == R) {
                    if (side) {
                        can_white_k_castle = false;
                    } else {
                        can_black_k_castle = false;
                    }
                }

                if (board.pieces[u.start] == K) {
                    if (side) {
                        can_white_k_castle = false;
                        can_white_q_castle = false;
                    } else {
                        can_black_k_castle = false;
                        can_black_q_castle = false;
                    }
                }

                if (u.k_castle || u.q_castle) {
                    if (side) {
                        can_white_k_castle = false;
                        can_white_q_castle = false;
                    } else {
                        can_black_k_castle = false;
                        can_black_q_castle = false;
                    }
                }

                side = (side) ? Black : White;
            } else {
                std::cout << "invalid move, try again\n";
            }
        }

        print_bitboards(board);
    }

    return 0;
}