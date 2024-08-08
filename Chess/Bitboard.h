//
// Created by Toby- on 10/02/2024.
//

#ifndef PROJECTREDO_BITBOARD_H
#define PROJECTREDO_BITBOARD_H

#include "Definitions.h"

#include <iostream>

//returns 0 if chosen square has 0, and 1 if chosen square has 1
bool getBit(bitboard bitboard, int square);

//set a chosen bit to 1
void setBit(bitboard &bitboard, int square);

//clear a chosen bit to 0
void clearBit(bitboard &bitboard, int square);

//prints out a bitboard in an 8x8 square
void printBitboard(bitboard &bitboard);

//count number of bits in bitboard
int countBits(bitboard bitboard);

//returns the index of the least significant bit of any bitboard
int getLeastSignificantBit(bitboard bitboard);


#endif //PROJECTREDO_BITBOARD_H
