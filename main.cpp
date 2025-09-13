#include <iostream>
#include <string>
#include <chrono> // deleteme
#include <thread> // deleteme

#include "protos.hpp"
#include "cli.hpp"

// void tweaker_random_js(Board &board) {
//     for (int lim = 0; lim < 10; lim++) {
//     // for (;;) {
//         bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
//         bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

//         Move m = random_guess(board, moved_2_spaces, k_uacamole, q_uacamole, moved_2_spaces, side);
//         Piece piece = board.pieces[m.start];
//         Piece captured = board.pieces[m.end];

//         board = make_move(board, m, side);
//         update_global_states(m, piece, captured, side);

//         debug_bbs(board);
//         print_pieces(board);

//         side = (side) ? Black : White;
//         if (decisive_result) break;
        
//         std::this_thread::sleep_for(std::chrono::milliseconds(200));
//     }
// }

void test_move_generation(Board curr_pos, Side side) {
    std::vector<Move> moves = extract_moves(curr_pos, side);
    for (Move move : moves) { // we all die in the end don't we
        Board fo_sho = make_move(curr_pos, move, side);
        print_pieces(fo_sho);
    }

    printf("Summary:\n");
    printf("%zu Moves Found\n", moves.size());
    printf("0 Life Prospects\n");
    printf("0 Financial Stability\n");
}

void test_min_max(Board curr_pos, Side side, i32 depth) {
    printf("searching at depth %d...\n", depth);
    Move m = min_max(curr_pos, side, depth);
    print_pieces(make_move(curr_pos, m, side));
    printf("considered %llu moves...\n", num_moves_considered);
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
        } else if (input == "t") {
            test_move_generation(board, side);
            continue;
        } else if (input == "m") {
            test_min_max(board, side, 4);
            continue;
        }

        if (valid_str(input)) {
            Move m = str_to_move(board, input);
            
            // temporary fix continues
            // bool k_uacamole = (side) ? can_white_k_castle : can_black_k_castle;
            // bool q_uacamole = (side) ? can_white_q_castle : can_black_q_castle;

            if (is_move_legal(board, m, side)) {
                Piece piece = board.pieces[m.start];
                Piece captured = board.pieces[m.end];

                board = make_move(board, m, side);
                // update_global_states(m, piece, captured, side);

                // debug_bbs(board);
                print_pieces(board);
                // debug_board(board);

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