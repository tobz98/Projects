//
// Created by Toby- on 10/02/2024.
//

#ifndef PROJECTREDO_MOVELIST_H
#define PROJECTREDO_MOVELIST_H

#include "Definitions.h"
#include "Move.h"
#include <vector>
#include <iostream>
#include <algorithm>

class MoveList {
private:
    std::vector<Move> moveList;

public:
    //constructor
    MoveList() = default;

    //add move to the movelist
    void addMove(Move move) {
        moveList.push_back(move);
    };

    //get the move with the given index
    Move getMoveIndex(int& index) {
        Move move = moveList[index];
        return move;
    };

    Move getMove(int initialSquare, int destinationSquare) {
        Move theMove = Move();
        //get moveList size to iterate over
        int size = getSize();
        //get actual move if it exists
        for(int i = 0; i < size; i++) {
            if(getMoveIndex(i).getInitialSquare() == initialSquare &&
               getMoveIndex(i).getDestinationSquare() == destinationSquare)
                theMove = getMoveIndex(i);
        }
        return theMove;
    }

    Move getMovePromo(int initialSquare, int destinationSquare, int promoPiece) {
        Move theMove = Move();
        //get moveList size to iterate over
        int size = getSize();
        //get actual move if it exists
        for(int i = 0; i < size; i++) {
            if(getMoveIndex(i).getInitialSquare() == initialSquare &&
               getMoveIndex(i).getDestinationSquare() == destinationSquare
               && getMoveIndex(i).getPromotionPiece() == promoPiece)
                theMove = getMoveIndex(i);
        }
        return theMove;
    }

    //print out all moves in the movelist
    void printMoves() {
        std::cout << "move: " << " side: " << " piece: " << " promotion piece: " << " capture: " <<" doublePawnPush "
                  << " enpassant: " << " castling: " << "\n";
        for(int i = 0; i < moveList.size(); i++) {
            moveList[i].printMove();
        }
    };

    //erase all moves in the movelist
    void eraseMoves() {
        moveList.clear();
    };

    //returns the number of moves in the movelist
    int getSize() {
        return (int)(moveList.size());
    }


    void sortMoveList() {
        sort(moveList.begin(), moveList.end());
    }
};



#endif //PROJECTREDO_MOVELIST_H
