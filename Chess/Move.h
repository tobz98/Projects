//
// Created by Toby- on 10/02/2024.
//

#ifndef PROJECTREDO_MOVE_H
#define PROJECTREDO_MOVE_H

#include "Definitions.h"

class Move {

private:
    int initialSquare;
    int destinationSquare;
    int side = white;
    int piece = pawn;
    int promotionPiece = 0;
    int capture = 0;
    int doublePawnPush = 0;
    int enpassant = 0;
    int castling = 0;

public:
    //constructors
    Move();

    Move(int initialSquare, int destinationSquare);

    Move(int initialSquare, int destinationSquare, int side, int piece, int promotionPiece, int capture,
         int doublePawnPush, int enpassant, int castling);

    Move(const Move& other);

    //copy another moves
    void copy(const Move& other);

    //assignment overload
    Move& operator=(const Move& other) {
        if (this != &other) {
            initialSquare = other.initialSquare;
            destinationSquare = other.destinationSquare;
            side = other.side;
            piece = other.piece;
            promotionPiece = other.promotionPiece;
            capture = other.capture;
            doublePawnPush = other.doublePawnPush;
            enpassant = other.enpassant;
            castling = other.castling;
        }
        return *this;
    };

    //prints out the move in form initialSquare destinationSquare eg e3e4
    void printMove() const;

    //set a move to the given values
    void setMove(int initialSquare, int destinationSquare, int side, int piece, int promotionPiece, int capture,
                 int doublePawnPush, int enpassant, int castling);

    //getter methods
    [[nodiscard]] int getInitialSquare() const {
        return initialSquare;
    };
    [[nodiscard]] int getDestinationSquare() const {
        return destinationSquare;
    };
    [[nodiscard]] int getSide() const {
        return side;
    };
    [[nodiscard]] int getPiece() const {
        return piece;
    };
    [[nodiscard]] int getPromotionPiece() const {
        return promotionPiece;
    };
    [[nodiscard]] int getCapture() const {
        return capture;
    };
    [[nodiscard]] int getDoublePawnPush() const {
        return doublePawnPush;
    };
    [[nodiscard]] int getEnpassant() const {
        return enpassant;
    };
    [[nodiscard]] int getCastling() const {
        return castling;
    };

    bool operator<(const Move& other) const {
        return (this->capture > other.capture);
    }
};




#endif //PROJECTREDO_MOVE_H
