//
// Created by Toby- on 08/08/2024.
//

#ifndef LEETCODE_GAME_H
#define LEETCODE_GAME_H
#include <vector>
#include <iostream>
#include <cstring>

const char EMPTY = ' ';
const char CROSS = 'X';
const char NOUGHT = 'O';

struct move {
    int x;
    int y;
};

static void printMove(move& move) {
    std::cout << "Move: (" << move.x << ", " << move.y << ")\n";
}

class Game {
public:

    char side = NOUGHT;
    move bestMove;

    //[x][y]
    char board[3][3] = {{EMPTY, EMPTY, EMPTY},
                        {EMPTY, EMPTY, EMPTY},
                        {EMPTY, EMPTY, EMPTY}};


    void printBoard() {
        for(int i = 2; i >= 0; i--) {
            std::cout << i << "  ";
            for(int j = 0; j <= 2; j++) {
                std::cout << "| " << board[j][i] << " |";
                if(j == 2)
                    std::cout << "\n";
            }
        }
        std::cout << "     0    1    2\n";
    }

    void switchSide() {
        if(side == CROSS)
            side = NOUGHT;
        else
            side = CROSS;
    }

    void makeMove(move& move) {
        board[move.x][move.y] = side;
        switchSide();
    }

    void unmakeMove(move& move) {
        board[move.x][move.y] = EMPTY;
        switchSide();
    }

    int legalMoveCheck(move& move) {
        //check if square is empty
        if(board[move.x][move.y] == EMPTY)
            return 1;
        return 0;
    }

    void genMoves(std::vector<move> &moves) {
        move move;
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++) {
                move.x = i;
                move.y = j;
                if(legalMoveCheck(move)) {
                    moves.push_back(move);
                }
            }
    }

    bool isDiagonalWin() {
        if(board[1][1] == EMPTY)
            return false;

        if(board[0][0] == board[1][1] &&
           board[1][1] == board[2][2])
            return true;

        if(board[2][0] == board[1][1] &&
           board[1][1] == board[0][2])
            return true;

        return false;
    }

    bool isVerticalWin() {

        for(int x = 0; x < 3; x++) {
            if(board[x][0] != EMPTY &&
               board[x][0] == board[x][1] &&
               board[x][1] == board[x][2])
                return true;
        }

        return false;
    }

    bool isHorizontalWin() {

        for(int y = 0; y < 3; y++) {
            if(board[0][y] != EMPTY &&
               board[0][y] == board[1][y] &&
               board[1][y] == board[2][y])
                return true;
        }

        return false;
    }

    bool isWin() {
        return isDiagonalWin() || isHorizontalWin() || isVerticalWin();
    }

    //minimax
    int minimax(int depth) {
        //score for position
        int score;

        //generate moves
        std::vector<move> moves;
        genMoves(moves);

        //base cases - win or no moves
        if(isWin()) {
            //if side == nought, then cross has just made a winning move, so return minimiser score
            if(side == NOUGHT) {
                return depth - 10;
            }
            else {
                return 10 - depth;
            }
        }

        if(moves.empty()) {
            return 0;
        }

        //minimiser
        if(side == CROSS) {
            score = 100;
            for(auto &move : moves) {
                makeMove(move);
                int childScore = minimax(depth + 1);

                if(childScore < score) {
                    score = childScore;
                    if(depth == 0)
                        bestMove = move;
                }
                unmakeMove(move);
            }
            return score;
        }

        //maximiser
        if(side == NOUGHT) {
            score = -100;
            for(auto &move : moves) {
                makeMove(move);
                int childScore = minimax(depth + 1);

                if(childScore > score) {
                    score = childScore;
                    if(depth == 0)
                        bestMove = move;
                }
                unmakeMove(move);
            }
            return score;
        }
    }

    //minimax
    int negamax(int depth) {
        //score for position
        int score;

        //generate moves
        std::vector<move> moves;
        genMoves(moves);

        //base cases - win or no moves
        if(isWin()) {
            return depth - 10;
        }

        if(moves.empty()) {
            return 0;
        }

        score = -100;
        for(auto &move : moves) {
            makeMove(move);
            int childScore = -negamax(depth + 1);

            if(childScore > score) {
                score = childScore;
                if(depth == 0)
                    bestMove = move;
            }
            unmakeMove(move);
        }

        return score;
    }


    void makeCompMove() {
        negamax(0);
        makeMove(bestMove);
    }

};



#endif //LEETCODE_GAME_H
