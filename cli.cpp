#include <map>

#include "cli.hpp"

// notes: having this const breaks [] operator for some reason
std::map<uint8_t, std::string> itoa = {
    {  0, " "}, 
    { wP, "♟"}, 
    { wN, "♞"}, 
    { wK, "♚"}, 
    { wQ, "♛"}, 
    { wB, "♝"}, 
    { wR, "♜"}, 
    { bP, "♙"}, 
    { bN, "♘"}, 
    { bK, "♔"}, 
    { bQ, "♕"}, 
    { bB, "♗"}, 
    { bR, "♖"}, 
};

// todo: somehow clear terminal, so it looks like stationary chess board
// todo: also allow quick refresh so it looks like timer is counting down
// todo: is printing out the board inefficient? also does it matter if it is?
void print_board() {
    printf("+---+---+---+---+---+---+---+---+\n");
    for (int i = 7; i >= 0; i--) {
        printf("| ");
        for (int j = 0; j < 8; j++) {
            // todo: WTF IS THIS ERROR?
            printf("%s | ", itoa[pieces[i * 8 + j]].c_str());
        }
        printf("\n");
        printf("+---+---+---+---+---+---+---+---+\n");
    }
}

// todo: check valid string
// todo: eventually accept algebraic notation
// todo: debug ts
void user_move(std::string curr) {
    move res;
    res.from = (curr[1] - '1') * 8 + (curr[0] - 'a');
    res.to = (curr[3] - '1') * 8 + (curr[2] - 'a');
    res.promote = 0;
    bool legal = make_move(res);

    if (legal) {
        printf("move made: %s\n", curr.c_str());
    } else {
        printf("illegal move\n");
    }
}