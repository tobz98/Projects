//
// Created by Toby- on 10/02/2024.
//

#ifndef PROJECTREDO_BOARD_H
#define PROJECTREDO_BOARD_H

#include "Definitions.h"
#include "Move.h"
#include "MoveList.h"

#include <chrono>

//initialise zorbist values
void initZorbist();

class Board {
private:

    //2d array for all individual piece bitboards
    bitboard pieces[2][6];

    //array of bitboards for all combined pieces
    bitboard occupancies[3];

    //side to move
    int sideToMove;

    //enpassant square
    int enpassant;

    //castling rights
    int castlingRights = 15;

    //position key
    bitboard positionKey = 0;

    //last move
    Move lastMove;

    //50 move counter
    int fiftyMove;

    //leaf node visited count for pert testing
    int nodesVisited = 0;

public:

    //constructor
    Board();

    //copy constructor
    Board(const Board& other);

    //restore board state using copy, used to undo a move
    void restore(const Board& other);

    //reads FEN board representation string and converts the board into that position
    void readFEN(const std::string &FEN);

    //convert current board position into a FEN string
    std::string convertToFEN();

    //setter method for all occupancies
    void setAllPieces(bitboard all) {
        occupancies[both] = all;
    };

    bitboard getPieceBitboard(int colour, int piece) {
        return pieces[colour][piece];
    }

    //getter methods for white pieces
    bitboard getWhitePawns() {
        return pieces[white][pawn];
    };

    bitboard getWhiteKnights() {
        return pieces[white][knight];
    };

    bitboard getWhiteBishops() {
        return pieces[white][bishop];
    };

    bitboard getWhiteRooks() {
        return pieces[white][rook];
    };

    bitboard getWhiteQueens() {
        return pieces[white][queen];
    };

    bitboard getWhiteKing() {
        return pieces[white][king];
    };

    //getter methods for black pieces
    bitboard getBlackPawns() {
        return pieces[black][pawn];
    };

    bitboard getBlackKnights() {
        return pieces[black][knight];
    };

    bitboard getBlackBishops() {
        return pieces[black][bishop];
    };

    bitboard getBlackRooks() {
        return pieces[black][rook];
    };

    bitboard getBlackQueens() {
        return pieces[black][queen];
    };

    bitboard getBlackKing() {
        return pieces[black][king];
    };

    //getters for occupancies
    bitboard getWhitePieces() {
        return occupancies[white];
    };

    bitboard getBlackPieces() {
        return occupancies[black];
    };

    bitboard getAllPieces() {
        return occupancies[both];
    };

    //getter method for side to move
    int getSideToMove() {
        return sideToMove;
    };

    //getter method for enpassant
    int getEnpassant() {
        return enpassant;
    };

    //getter method for castling rights
    int getCastlingRights() {
        return castlingRights;
    };

    //getter method for last move made
    Move getLastMove() {
        return lastMove;
    };

    //getter method for position key
    bitboard getPositionKey() {
        return positionKey;
    };

    //getter method for fifty move rule
    int getFiftyMove() {
        return fiftyMove;
    }

    //getter method for nodes visited (pert testing)
    int getNodesVisited() {
        return nodesVisited;
    }

    //getter method for move list
//    MoveList getMoveList() {
//        return allMoves;
//    };




    //MOVE GENERATION-------------------------------------------------------------------------------------

    //generates all castling moves in the position
    void genCastlingMoves(MoveList &list);

    //generates all knight moves in the position
    void genKnightMoves(MoveList &list);

    //generates all bishop moves in the position
    void genBishopMoves(MoveList &list);

    //generates all rook moves in the position
    void genRookMoves(MoveList &list);

    //generates all queen moves in the position
    void genQueenMoves(MoveList &list);

    //generates all king moves in the position
    void genKingMoves(MoveList &list);

    //generates all white pawn moves in the position
    void genWhitePawnMoves(MoveList &list);

    //generates all black pawn moves in the position
    void genBlackPawnMoves(MoveList &list);

    //generates all moves in the position
    void genMoves(MoveList &list);

    //clear move list
    void clearMoveList();

    //--------------------------------------------------------------------------------------------------

    //play move on board whether its legal or not
    void playMove(const Move &move);


    //make move on board, returns 1 if legal and 0 if not.
    int makeMove(const Move &move);

    //handling different moves in make move
    void handleCapture(int side, int oppositeSide, int destinationSquare, int capture);
    void handlePromotion(int side, int piece, int promotionPiece, int destinationSquare);
    void handleEnPassant(int side, int destinationSquare);
    void handleCastling(int side, int destinationSquare);
    void updateBoardState(int initialSquare, int destinationSquare, int side, int piece, int doublePush);
//------------------------------------------------------------------------
    //check if given square is attacked by given side, return 1 if attacked 0 if not
    int checkSquareAttacked(int square, int side);

    //print the board state, with capital letters for white and lower case for black eg P = white pawn, p = black pawn
    void printBoard();

    //check whether a move in the pseudo legal move list is legal or not, return 1 if legal, 0 if not
    int legalMoveCheck(Move move);

    void genLegalMoves(MoveList &list);

    //Evaluation Function------------------------------------------------------------------------------------
    int evaluate();

    //perft testing
    void perftTest(int depth);

    //Negaxmax search of best move
    int negamaxSearch(int depth, int alpha, int beta, Move &chosenMove);

    int negamaxSearchTime(int depth, int alpha, int beta, Move &chosenMove, int time, std::chrono::time_point<std::chrono::high_resolution_clock>& startTime);

    Move idNegamax(int maxDepth, int time);


    //engine function
    void engine(int depth, int time);


    //generate position key
    bitboard generatePosKey();

    //count number of times position is in positions vector
    int countPositions();
};


#endif //PROJECTREDO_BOARD_H
