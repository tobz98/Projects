//
// Created by Toby on 19/02/2024.
//

#include "Evaluation.h"

const int pawnScores[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
         5,  5, 10, 25, 25, 10,  5,  5,
         0,  0,  0, 25, 35,  0,  0,  0,
         5, 25,  0,-10,-10,  0, 25,  5,
         5, 30, 10,-20,-20, 10, 30,  5,
         0,  0,  0,  0,  0,  0,  0,  0
};

const int knightScores[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50
};

const int bishopScores[64] = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 20, 10, 10, 20,  0,-10,
        -10, 10, 10, 15, 15, 10, 10,-10,
        -10, 30,  5, 10, 10,  5, 30,-10,
        -20,-20,-20,-20,-20,-20,-20,-20
};

const int rookScores[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
         5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -10, 0,  0, 20, 20,  0,  0, -20
};

const int queenScores[64] = {
        -20,0,0,0,0,0,0,-20,
        -20,-20,-20,-20,-20,-20,-20,-20,
        -20,-20,-20,-20,-20,-20,-20,-20,
        -20,-20,-20,-20,-20,-20,-20,-20,
        -20,-20,-20,-20,-20,-20,-20,-20,
        -20,-20,-20,-20,-20,-20,-20,-20,
        -20,0,0,10,20,0,0,-20,
        -20,0,0,0,0,0,0,-20
};

const int kingScores[64] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   5,   5,   5,   5,   0,   0,
        0,   5,   5,  10,  10,   5,   5,   0,
        0,   5,  10,  20,  20,  10,   5,   0,
        0,   5,  10,  20,  20,  10,   5,   0,
        0,   0,   5,  10,  10,   5,   0,   0,
        0,   5,   5,  -5,  -5,   0,   5,   0,
        0,   0,  30, -20,   0, -20,  30,   0
};

const int mirrorScores[64] = {
        a8, b8, c8 ,d8 ,e8 ,f8, g8, h8,
        a7, b7, c7 ,d7 ,e7 ,f7, g7, h7,
        a6, b6, c6 ,d6 ,e6 ,f6, g6, h6,
        a5, b5, c5 ,d5 ,e5 ,f5, g5, h5,
        a4, b4, c4 ,d4 ,e4 ,f4, g4, h4,
        a3, b3, c3 ,d3 ,e3 ,f3, g3, h3,
        a2, b2, c2 ,d2 ,e2 ,f2, g2, h2,
        a1, b1, c1 ,d1 ,e1 ,f1, g1, h1
};

int pawnScoreWhite(bitboard pawns) {
    int square;
    int pawnPoints = 0;
    while(pawns) {
        square = getLeastSignificantBit(pawns);

        pawnPoints += 100;

        pawnPoints += pawnScores[mirrorScores[square]];

        clearBit(pawns, square);
    }
    return pawnPoints;
}

int pawnScoreBlack(bitboard pawns) {
    int square;
    int pawnPoints = 0;
    while(pawns) {
        square = getLeastSignificantBit(pawns);

        pawnPoints += 100;

        pawnPoints += pawnScores[square];

        clearBit(pawns, square);
    }
    return pawnPoints;
}

int knightScoreWhite(bitboard knights) {
    int square;
    int knightPoints = 0;
    while(knights) {
        square = getLeastSignificantBit(knights);
        knightPoints += 300;

        knightPoints += knightScores[mirrorScores[square]];

        clearBit(knights, square);
    }
    return knightPoints;
}

int knightScoreBlack(bitboard knights) {
    int square;
    int knightPoints = 0;
    while(knights) {
        square = getLeastSignificantBit(knights);
        knightPoints += 300;

        knightPoints += knightScores[square];

        clearBit(knights, square);
    }
    return knightPoints;
}

int bishopScoreWhite(bitboard bishops) {
    int square;
    int bishopPoints = 0;
    while(bishops) {
        square = getLeastSignificantBit(bishops);
        bishopPoints += 300;

        bishopPoints += bishopScores[mirrorScores[square]];

        clearBit(bishops, square);
    }
    return bishopPoints;
}

int bishopScoreBlack(bitboard bishops) {
    int square;
    int bishopPoints = 0;
    while(bishops) {
        square = getLeastSignificantBit(bishops);
        bishopPoints += 300;

        bishopPoints += bishopScores[square];

        clearBit(bishops, square);
    }
    return bishopPoints;
}

int rookScoreWhite(bitboard rooks) {
    int square;
    int rookPoints = 0;
    while(rooks) {
        square = getLeastSignificantBit(rooks);
        rookPoints += 500;

        rookPoints += rookScores[mirrorScores[square]];

        clearBit(rooks, square);
    }
    return rookPoints;
}

int rookScoreBlack(bitboard rooks) {
    int square;
    int rookPoints = 0;
    while(rooks) {
        square = getLeastSignificantBit(rooks);
        rookPoints += 500;

        rookPoints += rookScores[square];

        clearBit(rooks, square);
    }
    return rookPoints;
}

int queenScoreWhite(bitboard queens) {
    int square;
    int queenPoints = 0;
    while(queens) {
        square = getLeastSignificantBit(queens);
        queenPoints += 900;

        queenPoints += queenScores[mirrorScores[square]];

        clearBit(queens, square);
    }
    return queenPoints;
}

int queenScoreBlack(bitboard queens) {
    int square;
    int queenPoints = 0;
    while(queens) {
        square = getLeastSignificantBit(queens);
        queenPoints += 900;

        queenPoints += queenScores[square];

        clearBit(queens, square);
    }
    return queenPoints;
}

int kingScoreWhite(bitboard kings) {
    int square;
    int kingPoints = 0;
    while(kings) {
        square = getLeastSignificantBit(kings);
        kingPoints += 10000;

        kingPoints += kingScores[mirrorScores[square]];

        clearBit(kings, square);
    }
    return kingPoints;
}

int kingScoreBlack(bitboard kings) {
    int square;
    int kingPoints = 0;
    while(kings) {
        square = getLeastSignificantBit(kings);
        kingPoints += 10000;

        kingPoints += kingScores[square];

        clearBit(kings, square);
    }
    return kingPoints;
}

int pieceScoreWhite(bitboard pawns, bitboard knights, bitboard bishops, bitboard rooks,
               bitboard queens, bitboard kings) {
    int score = 0;

    score += pawnScoreWhite(pawns);
    score += knightScoreWhite(knights);
    score += bishopScoreWhite(bishops);
    score += rookScoreWhite(rooks);
    score += queenScoreWhite(queens);
    score += kingScoreWhite(kings);

    return score;
}

int pieceScoreBlack(bitboard pawns, bitboard knights, bitboard bishops, bitboard rooks,
                    bitboard queens, bitboard kings) {
    int score = 0;

    score += pawnScoreBlack(pawns);
    score += knightScoreBlack(knights);
    score += bishopScoreBlack(bishops);
    score += rookScoreBlack(rooks);
    score += queenScoreBlack(queens);
    score += kingScoreBlack(kings);

    return score;
}