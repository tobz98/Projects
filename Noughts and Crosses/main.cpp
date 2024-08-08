#include <iostream>
#include <unordered_map>
#include "Game.h"

Game board;

void beginGame() {
    std::string answer;


    std::cout << "Would you like to be 'NOUGHTS' or 'CROSSES'?\n";
    std::cin >> answer;

    while(answer != "CROSSES" && answer != "NOUGHTS") {
        std::cout << "Please enter a valid option: 'NOUGHTS' or 'CROSSES'.\n";
        std::cin >> answer;
    }

    if(answer == "CROSSES")
        board.side = CROSS;
    else
        board.side = NOUGHT;

    std::cout << "Would you like to go first '1' or second '2'?\n";
    std::cin >> answer;

    while(answer != "1" && answer != "2") {
        std::cout << "Please enter a valid option: 1 or 2.\n";
        std::cin >> answer;
    }
    if(answer == "2") {
        if(board.side == CROSS)
            board.side = NOUGHT;
        else
            board.side = CROSS;

        std::cout << "Game Beginning: \n";
        board.printBoard();
        board.makeCompMove();
        board.printBoard();
    }
    else {
        std::cout << "Game Beginning: \n";
        board.printBoard();
    }


}

move getPlayerMove() {
    int x, y;
    move move;

    std::cout << "Please enter your move x coord: \n";

    std::cin >> x;

    std::cout << "Please enter your move y coord: \n";

    std::cin >> y;

    move.x = x;
    move.y = y;

    return move;
}

int main() {
    board.printBoard();

    beginGame();


    while(true) {
        move move = getPlayerMove();
        board.makeMove(move);
        board.printBoard();
        board.makeCompMove();
        board.printBoard();

    }

    return 0;
}
