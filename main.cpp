#include <string>

#include "board.hpp"
#include "cli.hpp"

// testing
#include <iostream>

int main() {
    init_board();
    std::cout << "loading magics...\n";
    generate_rook_magics();
    generate_bishop_magics();

    // debug_board(get_knight_moves(1, white_pieces | black_pieces));
    // for (;;) {
    //     std::cout << "User Move: ";
    //     std::string despair;
    //     std::cin >> despair;
    //     if (despair == "q") {
    //         break;
    //     } else if (despair == "p") {
    //         print_board();
    //         continue;
    //     }
    //     user_move(despair);
    //     print_board();
    // }
    return 0;
}