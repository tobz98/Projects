//
// Created by Toby- on 10/02/2024.
//

#ifndef PROJECTREDO_DEFINITIONS_H
#define PROJECTREDO_DEFINITIONS_H

#include <cstdint>
#include <string>
#include <vector>

//declaring a bitboard to be a 64bit integer
typedef uint64_t bitboard;

//assigning all squares a value that is to be used for indexing. a1 = 0, a2 = 1 and so on.
enum {
    a1, b1, c1 ,d1 ,e1 ,f1, g1, h1,
    a2, b2, c2 ,d2 ,e2 ,f2, g2, h2,
    a3, b3, c3 ,d3 ,e3 ,f3, g3, h3,
    a4, b4, c4 ,d4 ,e4 ,f4, g4, h4,
    a5, b5, c5 ,d5 ,e5 ,f5, g5, h5,
    a6, b6, c6 ,d6 ,e6 ,f6, g6, h6,
    a7, b7, c7 ,d7 ,e7 ,f7, g7, h7,
    a8, b8, c8 ,d8 ,e8 ,f8, g8, h8, noSquare
};

//assigning value of 0 to white, 1 to black and 2 to both for use in indexing
const int white = 0;
const int black = 1;
const int both = 2;

//assigning values to all pieces for use in indexing
const int pawn = 0;
const int knight = 1;
const int bishop = 2;
const int rook = 3;
const int queen = 4;
const int king = 5;

//binary encoding of castling rights for each side
const int whiteKingside = 0b0001; //1
const int whiteQueenside = 0b0010; //2
const int blackKingside = 0b0100; //4
const int blackQueenside = 0b1000; //8

//bitboard masks used for generating moves:
constexpr bitboard notAFile = 0xfefefefefefefefeULL;
constexpr bitboard notHFile = 0x7f7f7f7f7f7f7f7fULL;
constexpr bitboard notABFile = 0xfcfcfcfcfcfcfcfcULL;
constexpr bitboard notHGFile = 0x3f3f3f3f3f3f3f3fULL;

extern bitboard pawnAttackMasks[2][64];
extern bitboard knightAttackMasks[64];
extern bitboard kingAttackMasks[64];

//string representation for all squares
inline const std::string squareStrings[] {
        "a1", "b1", "c1" ,"d1" ,"e1" ,"f1", "g1", "h1",
        "a2", "b2", "c2" ,"d2" ,"e2" ,"f2", "g2", "h2",
        "a3", "b3", "c3" ,"d3" ,"e3" ,"f3", "g3", "h3",
        "a4", "b4", "c4" ,"d4" ,"e4" ,"f4", "g4", "h4",
        "a5", "b5", "c5" ,"d5" ,"e5" ,"f5", "g5", "h5",
        "a6", "b6", "c6" ,"d6" ,"e6" ,"f6", "g6", "h6",
        "a7", "b7", "c7" ,"d7" ,"e7" ,"f7", "g7", "h7",
        "a8", "b8", "c8" ,"d8" ,"e8" ,"f8", "g8", "h8", "no square"
};

//used for updating castling rights with bitwise & on board castling rights value
inline const int castlingRightValues[64] = {
        13, 15, 15, 15,  12, 15, 15, 14,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        7, 15, 15, 15, 3, 15, 15, 11
};

//arrays for piece position values, used in evaluation function
extern const int pawnScores[64];
extern const int knightScores[64];
extern const int bishopScores[64];
extern const int rookScores[64];
extern const int queenScores[64];
extern const int kingScores[64];
extern const int mirrorScores[64];

extern bitboard zorbistPieceKeys[2][6][64];
extern bitboard sideKey;

extern std::vector<bitboard> positions;


#endif //PROJECTREDO_DEFINITIONS_H
