//
// Created by Toby- on 10/02/2024.
//

#include "Bitboard.h"

bool getBit(bitboard bitboard, int square) {
    return bitboard & (1ULL << square);
}

void setBit(bitboard &bitboard, int square) {
    bitboard |= (1ULL << square);
}

void clearBit(bitboard &bitboard, int square) {
    bitboard &= ~(1ULL << square);
}

void printBitboard(bitboard &bitboard) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {

            int square = rank * 8 + file;

            //print rank number
            if (square % 8 == 0)
                std::cout << rank + 1 << "   ";

            //print 1 or 0 depending on if square is occupied
            if (getBit(bitboard, square)) {
                std::cout << "1 ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "    a b c d e f g h\n\n";

    std::cout << "decimal value: " << bitboard << "\n\n";
}

int countBits(bitboard bitboard) {
    int count = 0;

    while (bitboard) {
        bitboard &= bitboard - 1;
        count++;
    }
    return count;
}

int getLeastSignificantBit(bitboard bitboard) {

    if (bitboard) {
        //count number of bits before the first bit
        return countBits((bitboard & -bitboard) - 1);
    }
        //return -1 if no bits
    else
        return -1;
}