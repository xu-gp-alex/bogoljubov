#include <string>

#include "board.hpp"
#include "cli.hpp"

int main() {
    init_board();
    for (;;) {
        std::cout << "User Move: ";
        std::string despair;
        std::cin >> despair;
        if (despair == "q") {
            break;
        }
        user_move(despair);
        print_board();
    }
    return 0;
}