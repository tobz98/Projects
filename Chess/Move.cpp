//
// Created by Toby- on 10/02/2024.
//

#include "Move.h"
#include <iostream>

Move::Move() {
    initialSquare = a1;
    destinationSquare = a1;
}


Move::Move(int initialSquare, int destinationSquare) {
    this->initialSquare = initialSquare;
    this->destinationSquare = destinationSquare;
}

Move::Move(int initialSquare, int destinationSquare, int side, int piece, int promotionPiece, int capture,
           int doublePawnPush, int enpassant, int castling) {

    this->initialSquare = initialSquare;
    this->destinationSquare = destinationSquare;
    this->side = side;
    this->piece = piece;
    this->promotionPiece = promotionPiece;
    this->capture = capture;
    this->doublePawnPush = doublePawnPush;
    this->enpassant = enpassant;
    this->castling = castling;
}

Move::Move(const Move& other) {
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

void Move::copy(const Move &other) {
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

void Move::printMove() const {
    std::cout << squareStrings[initialSquare] << squareStrings[destinationSquare]
              << "   " << side
              << "      " << piece
              << "       " << promotionPiece
              << "                 " << capture
              << "         " << doublePawnPush
              << "               " << enpassant
              << "           " << castling;
    if(promotionPiece == 4)
        std::cout << "q";
    if(promotionPiece == 3)
        std::cout << "r";
    if(promotionPiece == 2)
        std::cout << "b";
    if(promotionPiece == 1)
        std::cout << "n";

    std::cout << "\n";
}

void Move::setMove(int initialSquare, int destinationSquare, int side, int piece, int promotionPiece, int capture,
                   int doublePawnPush, int enpassant, int castling) {
    this->initialSquare = initialSquare;
    this->destinationSquare = destinationSquare;
    this->side = side;
    this->piece = piece;
    this->promotionPiece = promotionPiece;
    this->capture = capture;
    this->doublePawnPush = doublePawnPush;
    this->enpassant = enpassant;
    this->castling = castling;
}