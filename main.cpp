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
    // std::cout << "- enter \"s\" to skip :skull:\n";

    Board board = get_new_board();
    debug_bbs(board);
    print_pieces(board);
    
    // debug_bb(get_moves(board, E1+8, -1, true, true, White), "kms");
    // Board defect = make_move(board, E1+8, E1+16, -1, true, true, White);
    // debug_bbs(defect);
    // return 0;
    for (;;) {
        std::cout << "User Move: ";
        std::string input;
        std::cin >> input;
        if (input == "q") {
            break;
        }
        // } else if (input == "r") {
        //     init_board();
        //     board = get_new_board();
        //     continue;
        // } else if (input == "s") {
        //     side = (side) ? Black : White;
        //     continue;
        // }

        if (valid_str(input)) {
            move u = str_to_move(input);

            if (board.pieces[u.start] == K && u.end - u.start == 2) u.k_castle = true;
            if (board.pieces[u.start] == K && u.start - u.end == 2) u.q_castle = true;
            
            bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
            bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

            //
            debug_bb(get_moves(board, u.start, en_peasant, true, true, side), "help me jesus");

            if (is_move_legal(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side, u.promote)) {
                if (board.pieces[u.end] == K) {
                    break;
                }

                Piece stupid = board.pieces[u.start];
                board = make_move(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side, u.promote);

                // make_move also takes castling rights? should the outside determine what boolean to pass in? (prolly yes)
                
                if (stupid == P && abs(ROW(u.start) - ROW(u.end)) == 2) {
                    std::cout << "en_passant is eligible\n";
                    en_peasant = COL(u.start);
                } else {
                    std::cout << "reset correctly\n";
                    en_peasant = -1;
                }

                if (COL(u.start) == 0 && stupid == R) {
                    std::cout << "queenside castle invalidated from rook move\n";
                    if (side) {
                        can_white_q_castle = false;
                    } else {
                        can_black_q_castle = false;
                    }
                }

                if (COL(u.start) == 7 && stupid == R) {
                    std::cout << "kingside castle invalidated from rook move\n";
                    if (side) {
                        can_white_k_castle = false;
                    } else {
                        can_black_k_castle = false;
                    }
                }

                if (stupid == K) {
                    std::cout << "castle invalidated from king move\n";
                    if (side) {
                        can_white_k_castle = false;
                        can_white_q_castle = false;
                    } else {
                        can_black_k_castle = false;
                        can_black_q_castle = false;
                    }
                }

                if (u.k_castle || u.q_castle) {
                    std::cout << "just castled\n";
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
                std::cout << "illegal move, try again\n";
            }
        } else {
            std::cout << "invalid move, try again\n";
        }

        debug_bbs(board);
        print_pieces(board);
    }

    if (side) {
        std::cout << "congratulations to WHITE on winning!\n";
    } else {
        std::cout << "congratulations to BLACK on winning!\n";
    }

    return 0;
}