//
// Created by Toby- on 10/02/2024.
//

#include "AttackMasks.h"

bitboard pawnAttackMasks[2][64];
bitboard knightAttackMasks[64];
bitboard kingAttackMasks[64];

bitboard generatePawnAttack(int square, int side) {

    //piece bitboard
    bitboard pawns = 0ULL;

    //attack map bitboard
    bitboard attacks = 0ULL;

    //set piece on the board
    setBit(pawns, square);

    //generate white pawn map
    if (side == white) {
        attacks |= (pawns << 7) & notHFile;
        attacks |= (pawns << 9) & notAFile;
    }
        //generate black pawn map
    else {
        attacks |= (pawns >> 7) & notAFile;
        attacks |= (pawns >> 9) & notHFile;
    }
    //return attack map
    return attacks;
}

void createPawnAttacks() {

    //attack map
    bitboard attack = 0ULL;

    //loop over all squares for both sides and add map to array
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 64; j++) {
            attack = generatePawnAttack(j, i);
            pawnAttackMasks[i][j] = attack;
        }
    }
}

bitboard generateKnightAttack(int square) {

    //piece bitboard
    bitboard knights = 0ULL;

    //generated attack map
    bitboard attacks = 0ULL;

    //set piece on the board
    setBit(knights, square);

    //generate knight attack map
    attacks |= (knights << 6) & notHGFile;

    attacks |= (knights << 10) & notABFile;

    attacks |= (knights << 15) & notHFile;

    attacks |= (knights << 17) & notAFile;

    attacks |= (knights >> 6) & notABFile;

    attacks |= (knights >> 10) & notHGFile;

    attacks |= (knights >> 15) & notAFile;

    attacks |= (knights >> 17) & notHFile;

    //return attack map
    return attacks;
}

void createKnightAttacks() {

    //attack map
    bitboard attack = 0ULL;

    //loop over all squares adding each attack map to array
    for (int i = 0; i < 64; i++) {
        attack = generateKnightAttack(i);
        knightAttackMasks[i] = attack;
    }
}

bitboard generateKingAttack(int square) {

    //piece bitboard
    bitboard kings = 0ULL;

    //generated attack map
    bitboard attacks = 0ULL;

    //set piece on the board
    setBit(kings, square);

    //generate king attack map
    attacks |= (kings << 1) & notAFile;
    attacks |= (kings << 7) & notHFile;
    attacks |= (kings << 8);
    attacks |= (kings << 9) & notAFile;

    attacks |= (kings >> 1) & notHFile;
    attacks |= (kings >> 7) & notAFile;
    attacks |= (kings >> 8);
    attacks |= (kings >> 9) & notHFile;

    //return attack bitboard
    return attacks;
}

void createKingAttacks() {

    //attack bitboard
    bitboard attack = 0ULL;

    //loop over each square, generating attack and adding to array
    for (int i = 0; i < 64; i++) {
        attack = generateKingAttack(i);
        kingAttackMasks[i] = attack;
    }
}

bitboard generateBishopAttack(int square, bitboard blockingPieces) {
    //attack bitboard
    bitboard attacks = 0ULL;

    //initialise ranks & files
    int rank;
    int file;

    //initialise target ranks & files
    int targetRank = square / 8;
    int targetFile = square % 8;
    //gen bishop attacks for each diagonal, looping over each square

    for (rank = targetRank + 1, file = targetFile - 1; rank <= 7 && file >= 0; rank++, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if (1ULL << (rank * 8 + file) & blockingPieces) {
            break;
        }
    }
    for (rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file <= 7; rank--, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if (1ULL << (rank * 8 + file) & blockingPieces) {
            break;
        }
    }
    for (rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if (1ULL << (rank * 8 + file) & blockingPieces) {
            break;
        }
    }

    for (rank = targetRank + 1, file = targetFile + 1; rank <= 7 && file <= 7; rank++, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if (1ULL << (rank * 8 + file) & blockingPieces) {
            break;
        }
    }

    //return attack map
    return attacks;
}

bitboard generateRookAttack(int square, bitboard blockingPieces) {
    //attack bitboard
    bitboard attacks = 0ULL;

    //initialise ranks & files
    int rank;
    int file;

    //initialise target ranks & files
    int targetRank = square / 8;
    int targetFile = square % 8;

    //gen rook attacks for each rank and file, looping over each square
    for (file = targetFile + 1; file <= 7; file++) {
        attacks |= (1ULL << (targetRank * 8 + file));
        if (1ULL << (targetRank * 8 + file) & blockingPieces) {
            break;
        }
    }

    for (rank = targetRank - 1; rank >= 0; rank--) {
        attacks |= (1ULL << (rank * 8 + targetFile));
        if (1ULL << (rank * 8 + targetFile) & blockingPieces) {
            break;
        }
    }

    for (rank = targetRank + 1; rank <= 7; rank++) {
        attacks |= (1ULL << (rank * 8 + targetFile));
        if (1ULL << (rank * 8 + targetFile) & blockingPieces) {
            break;
        }
    }

    for (file = targetFile - 1; file >= 0; file--) {
        attacks |= (1ULL << (targetRank * 8 + file));
        if (1ULL << (targetRank * 8 + file) & blockingPieces) {
            break;
        }
    }

    //return attack map
    return attacks;
}

bitboard generateQueenAttack(int square, bitboard blockingPieces) {
    //attack bitboard
    bitboard attacks = 0ULL;

    //queen movement is a combination of a rook and a bishop
    bitboard bishopAttacks = generateBishopAttack(square, blockingPieces);
    bitboard rookAttacks = generateRookAttack(square, blockingPieces);

    attacks |= (bishopAttacks | rookAttacks);

    //return attack map
    return attacks;
}