//
// Created by Toby- on 10/02/2024.
//

#ifndef PROJECTREDO_ATTACKMASKS_H
#define PROJECTREDO_ATTACKMASKS_H

#include "Definitions.h"
#include "Bitboard.h"

//generate pawn attack mask for a pawn on a specific square and colour.
bitboard generatePawnAttack(int square, int side);

//generate all pawn attack masks for each side and square on the board
void createPawnAttacks();

//generate knight attack mask for a specific square on the board
bitboard generateKnightAttack(int square);

//create all knight attack masks for each square on the board
void createKnightAttacks();

//generate king attack mask for a specific square on the board
bitboard generateKingAttack(int square);

//create all king attack masks for each square on the board
void createKingAttacks();

//generate bishop attack mask for a specific square
bitboard generateBishopAttack(int square, bitboard blockingPieces);

//generate rook attack mask for a specific square
bitboard generateRookAttack(int square, bitboard blockingPieces);

//generate queen attack mask for a specific square
bitboard generateQueenAttack(int square, bitboard blockingPieces);

#endif //PROJECTREDO_ATTACKMASKS_H
