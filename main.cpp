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
    print_pieces(board);
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
            move u = str_to_move(input);

            if (board.pieces[u.start] == K && u.end - u.start == 2) u.k_castle = true;
            if (board.pieces[u.start] == K && u.start - u.end == 2) u.q_castle = true;
            
            bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
            bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

            // testing deez nutz
            u64 holy_grail = get_moves(board, u.start, en_peasant, k_uacamole, q_uacamole, side);
            debug_bitboard(holy_grail);
            debug_bitboard(get_white_pawn_moves(E1+8, board.sides[White] | board.sides[Black]));

            if (is_move_legal(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side)) {
                if (board.pieces[u.end] == K) {
                    break;
                }
                board = make_move(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side);
                // make_move also takes castling rights? should the outside determine what boolean to pass in? (prolly yes)
                
                // pieces must be incorporated into Board struct first
                // if (COL(u.start) == 0 && board.pieces[u.start] == R) {
                //     if (side) {
                //         can_white_q_castle = false;
                //     } else {
                //         can_black_q_castle = false;
                //     }
                // }

                // if (COL(u.start) == 7 && board.pieces[u.start] == R) {
                //     if (side) {
                //         can_white_k_castle = false;
                //     } else {
                //         can_black_k_castle = false;
                //     }
                // }

                // if (board.pieces[u.start] == K) {
                //     if (side) {
                //         can_white_k_castle = false;
                //         can_white_q_castle = false;
                //     } else {
                //         can_black_k_castle = false;
                //         can_black_q_castle = false;
                //     }
                // }

                // if (u.k_castle || u.q_castle) {
                //     if (side) {
                //         can_white_k_castle = false;
                //         can_white_q_castle = false;
                //     } else {
                //         can_black_k_castle = false;
                //         can_black_q_castle = false;
                //     }
                // }

                side = (side) ? Black : White;
            } else {
                std::cout << "illegal move, try again\n";
            }
        } else {
            std::cout << "invalid move, try again\n";
        }

        print_bitboards(board);
        print_pieces(board);
    }

    if (side) {
        std::cout << "congratulations to WHITE on winning!\n";
    } else {
        std::cout << "congratulations to BLACK on winning!\n";
    }

    return 0;
}