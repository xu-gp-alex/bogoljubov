#include <iostream>
#include <string>
#include <chrono> // deleteme
#include <thread> // deleteme

#include "protos.hpp"
#include "cli.hpp"

void tweaker_random_js(Board &board) {
    for (int lim = 0; lim < 10; lim++) {
    // for (;;) {
        bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
        bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

        Move m = random_guess(board, moved_2_spaces, k_uacamole, q_uacamole, moved_2_spaces, side);
        Piece piece = board.pieces[m.start];
        Piece captured = board.pieces[m.end];

        board = make_move(board, m, side);
        update_global_states(m, piece, captured, side);

        debug_bbs(board);
        print_pieces(board);

        side = (side) ? Black : White;
        if (decisive_result) break;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void cli_game_loop() {
    std::cout << "\nWelcome to BOGOLJUBOV chess engine :wilted-rose:\n";
    std::cout << "- enter \"h\" for help\n";
    std::cout << "- enter \"q\" to quit\n";
    std::cout << "- enter \"r\" to random.js\n\n";

    init_data();
    Board board = get_new_board();

    for (;;) {
        std::cout << "User Move: ";
        std::string input;
        std::cin >> input;
        if (input == "q") {
            break;
        } else if (input == "h") {
            std::cout << "- move notation is just starting and ending square\n";
            std::cout << "- applies for captures or non-captures\n";
            std::cout << "- append 'q', 'r', 'n', or 'b' to move for promotions\n\n";
            continue;
        } else if (input == "r") {
            // tweaker_random_js(board);
            break;
        }

        if (valid_str(input)) {
            Move m = str_to_move(board, input);
            
            bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
            bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

            if (is_move_legal(board, m, side, k_uacamole, q_uacamole, moved_2_spaces)) {
                Piece piece = board.pieces[m.start];
                Piece captured = board.pieces[m.end];

                board = make_move(board, m, side);
                update_global_states(m, piece, captured, side);

                debug_bbs(board);
                print_pieces(board);

                side = (side) ? Black : White;
            } else {
                std::cout << "illegal move, try again\n";
            }
        } else {
            std::cout << "illegal input, try again\n";
        }

        if (decisive_result) break;
    }

    if (decisive_result) {
        if (side) {
            std::cout << "congratulations to WHITE on winning!\n";
        } else {
            std::cout << "congratulations to BLACK on winning!\n";
        }
    } else {
        std::cout << "no result\n";
    }
}

int main() {
    init_data();
    std::cout << "loading magics...\n";
    generate_rook_magics();
    generate_bishop_magics();

    cli_game_loop();
    return 0;
}

//     if (no_control) {
//         i32 limit = 400;
//         while (limit--) {
//             bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
//             bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

//             move u = random_guess(board, en_peasant, k_uacamole, q_uacamole, side);
//             if (u.start == null_move.start) {
//                 break;
//             }

//             if (board.pieces[u.start] == K && u.end - u.start == 2) u.k_castle = true;
//             if (board.pieces[u.start] == K && u.start - u.end == 2) u.q_castle = true;

//             Piece stupid = board.pieces[u.start];
//             Piece slimed = board.pieces[u.end];
//             board = make_move(board, u.start, u.end, en_peasant, k_uacamole, q_uacamole, side, u.promote);

//             // testing
//             // debug_bb(board.sides[White], "White");
//             // debug_bb(board.sides[Black], "Black");

//             // make_move also takes castling rights? should the outside determine what boolean to pass in? (prolly yes)
            
//             if (stupid == P && abs(ROW(u.start) - ROW(u.end)) == 2) {
//                 en_peasant = COL(u.start);
//             } else {
//                 en_peasant = -1;
//             }

//             if (COL(u.start) == 0 && stupid == R) {
//                 if (side) {
//                     can_white_q_castle = false;
//                 } else {
//                     can_black_q_castle = false;
//                 }
//             }

//             if (COL(u.start) == 7 && stupid == R) {
//                 if (side) {
//                     can_white_k_castle = false;
//                 } else {
//                     can_black_k_castle = false;
//                 }
//             }

//             if (stupid == K) {
//                 if (side) {
//                     can_white_k_castle = false;
//                     can_white_q_castle = false;
//                 } else {
//                     can_black_k_castle = false;
//                     can_black_q_castle = false;
//                 }
//             }

//             if (u.k_castle || u.q_castle) {
//                 if (side) {
//                     can_white_k_castle = false;
//                     can_white_q_castle = false;
//                 } else {
//                     can_black_k_castle = false;
//                     can_black_q_castle = false;
//                 }
//             }

//             debug_bbs(board);
//             print_pieces(board);

//             // lowk restarted placement...
//             if (slimed == K) {
//                 decisive_result = true;
//                 break;
//             }

//             side = (side) ? Black : White;
//             std::this_thread::sleep_for(std::chrono::milliseconds(50));
//         }
//     }
    
//     if (decisive_result) {
//         if (side) {
//             std::cout << "congratulations to WHITE on winning!\n";
//         } else {
//             std::cout << "congratulations to BLACK on winning!\n";
//         }
//     }

//     return 0;
// }