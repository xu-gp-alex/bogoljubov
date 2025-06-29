#include <string>

#include "board.hpp"
#include "cli.hpp"

// testing
#include <iostream>

int main() {
    init_board();
    generate_rook_magics();
    std::cout << "rook magics generated\n";
    debug_board(get_rook_moves(0, white_pieces | black_pieces));
    // for (int i = 0; i < 64; i++) {
    //     print_binary(f_bishop(i, 0));
    // }
    // init_board();
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