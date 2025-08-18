#include <iostream>
#include <string>
#include <map>
#include <unistd.h> // delete me

#include "protos.hpp"
#include "cli.hpp"

int main() {
    init_board();
    std::cout << "loading magics...\n";
    generate_rook_magics();
    generate_bishop_magics();

    // cli_game_loop()
    std::cout << "\nWelcome to BOGOLJUBOV chess engine :wilted-rose:\n";
    std::cout << "- enter \"h\" for help\n";
    std::cout << "- enter \"q\" to quit\n";
    std::cout << "- enter \"r\" to random.js\n\n";

    Board board = get_new_board();

    // testing
    // move yuh = random_guess(board, -1, true, true, side);
    // std::cout << "start: " << yuh.start << '\n';
    // std::cout << "end: " << yuh.end << '\n';
    // return 0;

    // lazy coding lol
    bool decisive_result = false;
    bool no_control = false;
    
    for (;;) {
        std::cout << "User Move: ";
        std::string input;
        std::cin >> input;
        if (input == "q") {
            break;
        } else if (input == "h") {
            std::cout << "- move notation is just starting and ending square\n";
            std::cout << "- applies for captures or non-captures\n";
            std::cout << "- add =Q, =R, =N, or =B for promotions\n\n";
            continue;
        } else if (input == "r") {
            no_control = true;
            break;
        }

        if (valid_str(input)) {
            move u = str_to_move(input);

            if (board.pieces[u.start] == K && u.end - u.start == 2) u.k_castle = true;
            if (board.pieces[u.start] == K && u.start - u.end == 2) u.q_castle = true;
            
            bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
            bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

            if (is_move_legal(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side, u.promote)) {
                Piece stupid = board.pieces[u.start];
                Piece slimed = board.pieces[u.end];
                board = make_move(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side, u.promote);

                // make_move also takes castling rights? should the outside determine what boolean to pass in? (prolly yes)
                
                if (stupid == P && abs(ROW(u.start) - ROW(u.end)) == 2) {
                    en_peasant = COL(u.start);
                } else {
                    en_peasant = -1;
                }

                if (COL(u.start) == 0 && stupid == R) {
                    if (side) {
                        can_white_q_castle = false;
                    } else {
                        can_black_q_castle = false;
                    }
                }

                if (COL(u.start) == 7 && stupid == R) {
                    if (side) {
                        can_white_k_castle = false;
                    } else {
                        can_black_k_castle = false;
                    }
                }

                if (stupid == K) {
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

                print_pieces(board);

                // lowk restarted placement...
                if (slimed == K) {
                    decisive_result = true;
                    break;
                }

                side = (side) ? Black : White;
            } else {
                std::cout << "illegal move, try again\n";
            }
        } else {
            std::cout << "invalid move, try again\n";
        }
    }

    if (no_control) {
        i32 limit = 20;
        while (limit--) {
            bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
            bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

            move u = random_guess(board, en_peasant, k_uacamole, q_uacamole, side);
            if (u.start == -1) {
                break;
            }

            Piece stupid = board.pieces[u.start];
            Piece slimed = board.pieces[u.end];
            board = make_move(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side, u.promote);

            // make_move also takes castling rights? should the outside determine what boolean to pass in? (prolly yes)
            
            if (stupid == P && abs(ROW(u.start) - ROW(u.end)) == 2) {
                en_peasant = COL(u.start);
            } else {
                en_peasant = -1;
            }

            if (COL(u.start) == 0 && stupid == R) {
                if (side) {
                    can_white_q_castle = false;
                } else {
                    can_black_q_castle = false;
                }
            }

            if (COL(u.start) == 7 && stupid == R) {
                if (side) {
                    can_white_k_castle = false;
                } else {
                    can_black_k_castle = false;
                }
            }

            if (stupid == K) {
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

            debug_bbs(board);
            print_pieces(board);

            // lowk restarted placement...
            if (slimed == K) {
                decisive_result = true;
                break;
            }

            side = (side) ? Black : White;
            sleep(1);
        }
    }
    
    if (decisive_result) {
        if (side) {
            std::cout << "congratulations to WHITE on winning!\n";
        } else {
            std::cout << "congratulations to BLACK on winning!\n";
        }
    }

    return 0;
}