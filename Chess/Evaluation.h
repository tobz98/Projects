//
// Created by Toby- on 19/02/2024.
//

#ifndef PROJECTREDO_EVALUATION_H
#define PROJECTREDO_EVALUATION_H

#include "Definitions.h"
#include "Bitboard.h"

// return the score for pawns in the position, each pawn is worth 100
int pawnScoreWhite(bitboard pawns);

// return the score for knights in the position, each knight is worth 300
int knightScoreWhite(bitboard knights);

// return the score for bishops in the position, each bishop is worth 300
int bishopScoreWhite(bitboard bishops);

// return the score for rooks in the position, each rook is worth 500
int rookScoreWhite(bitboard rooks);

// return the score for queens in the position, each queen is worth 900
int queenScoreWhite(bitboard queens);

// return the score for the king, king is worth 10000
int kingScoreWhite(bitboard kings);

//return the score for all pieces in a position
int pieceScoreWhite(bitboard pawns, bitboard knights, bitboard bishops, bitboard rooks,
               bitboard queens, bitboard kings);

// return the score for pawns in the position, each pawn is worth 100
int pawnScoreBlack(bitboard pawns);

// return the score for knights in the position, each knight is worth 300
int knightScoreBlack(bitboard knights);

// return the score for bishops in the position, each bishop is worth 300
int bishopScoreBlack(bitboard bishops);

// return the score for rooks in the position, each rook is worth 500
int rookScoreBlack(bitboard rooks);

// return the score for queens in the position, each queen is worth 900
int queenScoreBlack(bitboard queens);

// return the score for the king, king is worth 10000
int kingScoreBlack(bitboard kings);

//return the score for all pieces in a position
int pieceScoreBlack(bitboard pawns, bitboard knights, bitboard bishops, bitboard rooks,
                    bitboard queens, bitboard kings);

#endif //PROJECTREDO_EVALUATION_H
