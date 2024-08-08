//
// Created by Toby- on 10/02/2024.
//

#include "Board.h"
#include "AttackMasks.h"
#include "Move.h"
#include <sstream>
#include <unordered_set>
#include <GL/freeglut.h>
#include "Evaluation.h"

bitboard zorbistPieceKeys[2][6][64];

bitboard sideKey;

std::unordered_set<bitboard> positionKeys;

std::vector<bitboard> positions;

bitboard rand64() {
    return rand() ^ ((bitboard)rand() << 15) ^ ((bitboard)rand() << 30) ^
    ((bitboard)rand() << 45) ^ ((bitboard)rand() << 60);
}

void initZorbist() {
    for(int side = 0; side < 2; side++)
        for(int piece = 0; piece < 6; piece++)
            for(int square = 0; square < 64; square++) {
                zorbistPieceKeys[side][piece][square] = rand64();
            }
    sideKey = rand64();
}

Board::Board() {

    pieces[white][pawn] = 65280ULL;
    pieces[white][knight] = 66ULL;
    pieces[white][bishop] = 36ULL;
    pieces[white][rook] = 129ULL;
    pieces[white][queen] = 8ULL;
    pieces[white][king] = 16ULL;


    pieces[black][pawn] = 0xff000000000000ULL;
    pieces[black][knight] = 0x4200000000000000ULL;
    pieces[black][bishop] = 0x2400000000000000ULL;
    pieces[black][rook] = 0x8100000000000000ULL;
    pieces[black][queen] = 0x800000000000000ULL;
    pieces[black][king] = 0x1000000000000000ULL;

    occupancies[white] = pieces[white][pawn] | pieces[white][knight] |
                         pieces[white][bishop] | pieces[white][rook] |
                         pieces[white][queen] | pieces[white][king];

    occupancies[black] = pieces[black][pawn] | pieces[black][knight] |
                         pieces[black][bishop] | pieces[black][rook] |
                         pieces[black][queen] | pieces[black][king];

    occupancies[both] = occupancies[white] | occupancies[black];

    sideToMove = white;

    enpassant = noSquare;

    castlingRights = 15;

    fiftyMove = 0;

    positionKey = generatePosKey();

    positions.push_back(positionKey);
};

Board::Board(const Board& other) {
    //copy piece bitboards
    memcpy(pieces, other.pieces, sizeof(pieces));

    //copy occupancies
    memcpy(occupancies, other.occupancies, sizeof(occupancies));

    //copy fields
    sideToMove = other.sideToMove;
    enpassant = other.enpassant;
    castlingRights = other.castlingRights;
    positionKey = other.positionKey;
    fiftyMove = other.fiftyMove;
}

void Board::restore(const Board& other) {
    //copy piece bitboards
    memcpy(pieces, other.pieces, sizeof(pieces));

    //copy occupancies
    memcpy(occupancies, other.occupancies, sizeof(occupancies));

    //copy fields
    sideToMove = other.sideToMove;
    enpassant = other.enpassant;
    castlingRights = other.castlingRights;
    positionKey = other.positionKey;
    fiftyMove = other.fiftyMove;
}

void Board::readFEN(const std::string& FEN) {

    sideToMove = white;
    enpassant = noSquare;
    castlingRights = 0;

    //initialise all board bitboards to 0
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            pieces[i][j] = 0ULL;
        }
        occupancies[i] = 0ULL;
    }
    occupancies[both] = 0ULL;

    //split FEN into its 6 different parts and store them in a vector
    std::istringstream iss(FEN);
    std::vector<std::string> FENfields;
    std::string field;
    while (iss >> field) {
        FENfields.push_back(field);
    }

    //handling position, FENfields[0]
    int rank = 7;
    int file = 0;

    for (char c: FENfields[0]) {
        if (c == '/') {
            //move to the next rank
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            //skip empty squares
            file += (c - '0');
        } else {
            //set the piece on the board based on FEN character
            int colour = isupper(c) ? 0 : 1;
            int pieceType = 0;

            switch (tolower(c)) {
                case 'p':
                    pieceType = pawn;
                    break;
                case 'n':
                    pieceType = knight;
                    break;
                case 'b':
                    pieceType = bishop;
                    break;
                case 'r':
                    pieceType = rook;
                    break;
                case 'q':
                    pieceType = queen;
                    break;
                case 'k':
                    pieceType = king;
                    break;
            }

            //set the bit for the piece on the board
            pieces[colour][pieceType] |= (1ULL << (rank * 8 + file));

            //set occupancy bitboards
            occupancies[colour] |= (1ULL << (rank * 8 + file));

            file++;
        }
    }
    occupancies[both] |= (occupancies[white] | occupancies[black]);

    //handling colour field, FENfields[1]
    for (char c: FENfields[1]) {
        sideToMove = (c == 'w') ? white : black;
    }

    //handling castling rights
    for (char c: FENfields[2]) {
        switch (c) {
            case 'K': castlingRights |= whiteKingside; break;
            case 'Q': castlingRights |= whiteQueenside; break;
            case 'k': castlingRights |= blackKingside; break;
            case 'q': castlingRights |= blackQueenside; break;
            case '-': break;
        }
    }

    //handling enpassant field, FENfields[3]
    if(FENfields[3] != "-") {
        //generate enpassant file and rank values
        int enpFile = FENfields[3][0] - 'a';
        int enpRank = (FENfields[3][1] - '1');

        //create enpassant square value
        enpassant = enpRank * 8 + enpFile;
    }
    else
        enpassant = noSquare;
    positionKey = generatePosKey();
    positions.push_back(positionKey);
}

std::string Board::convertToFEN() {
    std::stringstream fen;

    //loop over all squares and find pieces
    for (int rank = 7; rank >= 0; rank--) {
        int emptySquares = 0;
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            int pieceType = -1;

            for (int colour = 0; colour < 2; colour++) {
                for (int piece = 0; piece < 6; piece++) {
                    if ((pieces[colour][piece] >> square) & 1) {
                        pieceType = piece;
                        break;
                    }
                }
                if (pieceType != -1) break;
            }

            if (pieceType == -1) {
                emptySquares++;
            } else {
                if (emptySquares > 0) {
                    fen << emptySquares;
                    emptySquares = 0;
                }

                char pieceChar;
                switch (pieceType) {
                    case 0: pieceChar = 'P'; break;
                    case 1: pieceChar = 'N'; break;
                    case 2: pieceChar = 'B'; break;
                    case 3: pieceChar = 'R'; break;
                    case 4: pieceChar = 'Q'; break;
                    case 5: pieceChar = 'K'; break;
                    default: pieceChar = '?'; break;
                }

                if (pieces[black][pieceType] & (1ULL << square)) {
                    pieceChar = tolower(pieceChar);
                }

                fen << pieceChar;
            }
        }

        if (emptySquares > 0) {
            fen << emptySquares;
        }

        if (rank > 0) {
            fen << '/';
        }
    }

    fen << ' ';

    //set side to move
    fen << (sideToMove == 0 ? 'w' : 'b');

    fen << ' ';

    //set castling rights
    if (castlingRights == 0) {
        fen << '-';
    } else {
        if (castlingRights & 1) fen << 'K';
        if (castlingRights & 2) fen << 'Q';
        if (castlingRights & 4) fen << 'k';
        if (castlingRights & 8) fen << 'q';
    }

    fen << ' ';

    //set enpassant square
    if (enpassant == noSquare) {
        fen << '-';
    } else {
        char fileChar = 'a' + (enpassant % 8);
        char rankChar = '1' + (enpassant / 8);
        fen << fileChar << rankChar;
    }

    return fen.str();
}

//--------------------------------making moves--------------------------------------------
int Board::legalMoveCheck(Move move) {
    Board copy = Board(*this);
    if(makeMove(move)) {
        this->restore(copy);
        return 1;
    }
    this->restore(copy);
    return 0;
}

void Board::playMove(const Move& move) {
    positionKeys.insert(positionKey);

    //retrieve move fields
    int initialSquare = move.getInitialSquare();
    int destinationSquare = move.getDestinationSquare();
    int side = move.getSide();
    int piece = move.getPiece();
    int promotionPiece = move.getPromotionPiece();
    int capture = move.getCapture();
    int doublePush = move.getDoublePawnPush();
    int enpass = move.getEnpassant();
    int castle = move.getCastling();

    //update fifty move rule
    fiftyMove = (capture || (piece == pawn)) ? 0 : fiftyMove + 1;

    //initialise opposite side
    int oppositeSide = (side == white) ? black : white;

    //move piece
    clearBit(pieces[side][piece], initialSquare);
    setBit(pieces[side][piece], destinationSquare);

    //set the position key
    positionKey ^= zorbistPieceKeys[side][piece][initialSquare];
    positionKey ^= zorbistPieceKeys[side][piece][destinationSquare];

    //handle captures
    if (capture) {
        handleCapture(side, oppositeSide, destinationSquare, capture);
    }

    //handle promotions
    if (promotionPiece) {
        handlePromotion(side, piece, promotionPiece, destinationSquare);
    }

    //handle enpassant
    if (enpass) {
        handleEnPassant(side, destinationSquare);
    }

    //handle castling
    if (castle) {
        handleCastling(side, destinationSquare);
    }

    //update board state
    updateBoardState(initialSquare, destinationSquare, side, piece, doublePush);

    //update other board fields
    sideToMove ^= 1;
    positionKey ^= sideKey;
    lastMove = move;
    positions.push_back(positionKey);
}

int Board::makeMove(const Move& move) {
    //copy board
    Board copy = Board(*this);

    //retrieve move fields
    int initialSquare = move.getInitialSquare();
    int destinationSquare = move.getDestinationSquare();
    int side = move.getSide();
    int piece = move.getPiece();
    int promotionPiece = move.getPromotionPiece();
    int capture = move.getCapture();
    int doublePush = move.getDoublePawnPush();
    int enpass = move.getEnpassant();
    int castle = move.getCastling();

    //update fifty move rule
    fiftyMove = (capture || (piece == pawn)) ? 0 : fiftyMove + 1;

    //initialise opposite side
    int oppositeSide = (side == white) ? black : white;

    //move piece
    clearBit(pieces[side][piece], initialSquare);
    setBit(pieces[side][piece], destinationSquare);

    //set position key
    positionKey ^= zorbistPieceKeys[side][piece][initialSquare];
    positionKey ^= zorbistPieceKeys[side][piece][destinationSquare];

    //handle captures
    if (capture) {
        handleCapture(side, oppositeSide, destinationSquare, capture);
    }

    //handle promotions
    if (promotionPiece) {
        handlePromotion(side, piece, promotionPiece, destinationSquare);
    }

    //handle enpassant
    if (enpass) {
        handleEnPassant(side, destinationSquare);
    }

    //handle castling
    if (castle) {
        handleCastling(side, destinationSquare);
    }

    //update board state
    updateBoardState(initialSquare, destinationSquare, side, piece, doublePush);

    //check if king is in check
    int kingSquare = getLeastSignificantBit(pieces[side][king]);

    if (checkSquareAttacked(kingSquare, oppositeSide)) {
        //restore board state
        this->restore(copy);

        //return 0 for illegal move
        return 0;
    }

    //update board fields
    sideToMove ^= 1;
    positionKey ^= sideKey;

    //return 1 for legal move
    return 1;
}

void Board::handleCapture(int side, int oppositeSide, int destinationSquare, int capture) {
    //check if there is a piece on the target square, then remove it
    for (int bitboardPiece = pawn; bitboardPiece <= king; bitboardPiece++) {
        if (getBit(pieces[oppositeSide][bitboardPiece], destinationSquare)) {
            clearBit(pieces[oppositeSide][bitboardPiece], destinationSquare);
            //set position key
            positionKey ^= zorbistPieceKeys[oppositeSide][bitboardPiece][destinationSquare];
            break;
        }
    }
}

void Board::handlePromotion(int side, int piece, int promotionPiece, int destinationSquare) {
    //remove pawn from destination square, and replace with chosen promoted piece
    clearBit(pieces[side][piece], destinationSquare);
    setBit(pieces[side][promotionPiece], destinationSquare);

    //set position key
    positionKey ^= zorbistPieceKeys[side][piece][destinationSquare];
    positionKey ^= zorbistPieceKeys[side][promotionPiece][destinationSquare];
}

void Board::handleEnPassant(int side, int destinationSquare) {
    //remove captured pawn
    if (side == white) {
        clearBit(pieces[black][pawn], destinationSquare - 8);
        //set position key
        positionKey ^= zorbistPieceKeys[black][pawn][destinationSquare - 8];
    }
    if (side == black) {
        clearBit(pieces[white][pawn], destinationSquare + 8);
        //set position key
        positionKey ^= zorbistPieceKeys[white][pawn][destinationSquare + 8];
    }
}

void Board::handleCastling(int side, int destinationSquare) {
    if (destinationSquare == g1) {
        //white castling king side
        clearBit(pieces[side][rook], h1);
        setBit(pieces[side][rook], f1);
        positionKey ^= zorbistPieceKeys[white][rook][h1];
        positionKey ^= zorbistPieceKeys[white][rook][f1];
    } else if (destinationSquare == c1) {
        //white castling queen side
        clearBit(pieces[side][rook], a1);
        setBit(pieces[side][rook], d1);
        positionKey ^= zorbistPieceKeys[white][rook][a1];
        positionKey ^= zorbistPieceKeys[white][rook][d1];
    } else if (destinationSquare == g8) {
        //black castling king side
        clearBit(pieces[side][rook], h8);
        setBit(pieces[side][rook], f8);
        positionKey ^= zorbistPieceKeys[black][rook][h8];
        positionKey ^= zorbistPieceKeys[white][rook][f8];
    } else if (destinationSquare == c8) {
        //black castling queen side
        clearBit(pieces[side][rook], a8);
        setBit(pieces[side][rook], d8);
        positionKey ^= zorbistPieceKeys[black][rook][a8];
        positionKey ^= zorbistPieceKeys[white][rook][d8];
    }
}

void Board::updateBoardState(int initialSquare, int destinationSquare, int side, int piece, int doublePush) {
    //set enpassant square after every move
    enpassant = doublePush ? ((side == white) ? destinationSquare - 8 : destinationSquare + 8) : noSquare;

    //update castling rights
    castlingRights &= castlingRightValues[initialSquare];
    castlingRights &= castlingRightValues[destinationSquare];

    //reset occupancies to zero
    for (int sides = 0; sides <= 2; sides++) {
        occupancies[sides] = 0ULL;
    }
    //reinitialize occupancies to their new values
    for (int sides = 0; sides < 2; sides++) {
        for (int pieceType = 0; pieceType < 6; pieceType++) {
            occupancies[sides] |= pieces[sides][pieceType];
        }
    }
    occupancies[both] |= (occupancies[white] | occupancies[black]);
}

//---------------------------------------------------------------------------------------------------------

void Board::genLegalMoves(MoveList &list) {
    MoveList legal;
    for(int i = 0; i < list.getSize(); i++) {

        if(legalMoveCheck(list.getMoveIndex(i)))
            legal.addMove(list.getMoveIndex(i));
    }
    list = legal;
}

int Board::checkSquareAttacked(int square, int side) {
    //check if square attacked by white king
    if ((side == white) && (kingAttackMasks[square] & pieces[white][king])) {
        return 1;
    }

    //check if square attacked by black king
    if ((side == black) && (kingAttackMasks[square] & pieces[black][king])) {
        return 1;
    }

    //check if square attacked by white queen
    if ((side == white) && (generateQueenAttack(square, occupancies[both]) & pieces[white][queen])) {
        return 1;
    }

    //check if square attacked by black queen
    if ((side == black) && (generateQueenAttack(square, occupancies[both]) & pieces[black][queen])) {
        return 1;
    }

    //check if square attacked by white rook
    if ((side == white) && (generateRookAttack(square, occupancies[both]) & pieces[white][rook])) {
        return 1;
    }

    //check if square attacked by black rook
    if ((side == black) && (generateRookAttack(square, occupancies[both]) & pieces[black][rook])) {
        return 1;
    }

    //check if square attacked by white bishop
    if ((side == white) && (generateBishopAttack(square, occupancies[both]) & pieces[white][bishop])) {
        return 1;
    }

    //check if square attacked by black bishop
    if ((side == black) && (generateBishopAttack(square, occupancies[both]) & pieces[black][bishop])) {
        return 1;
    }

    //check if square attacked by white knight
    if ((side == white) && (knightAttackMasks[square] & pieces[white][knight])) {
        return 1;
    }

    //check if square attacked by black knight
    if ((side == black) && (knightAttackMasks[square] & pieces[black][knight])) {
        return 1;
    }

    //check if square attacked by white pawn
    if ((side == white) && (pawnAttackMasks[black][square] & pieces[white][pawn])) {
        return 1;
    }

    //check if square attacked by black pawn
    if ((side == black) && (pawnAttackMasks[white][square] & pieces[black][pawn])) {
        return 1;
    }

    //square is not attacked
    return 0;
}

//MOVE GENERATION-------------------------------------------------------------------------------------

void Board::genCastlingMoves(MoveList &list) {

    //initiate move
    Move move;

    //initialise values
    int oppositeSide, b, c, d, e, f, g, kingside, queenside;

    if(sideToMove == white) {
        oppositeSide = black;
        b = b1, c = c1, d = d1, e = e1, f = f1, g = g1;
        kingside = whiteKingside;
        queenside = whiteQueenside;
    } else {
        oppositeSide = white;
        b = b8, c = c8, d = d8, e = e8, f = f8, g = g8;
        kingside = blackKingside;
        queenside = blackQueenside;
    }

    //check if kingside castling is available
    if (castlingRights & kingside) {
        //check squares between king and rook are empty
        if (!getBit(occupancies[both], f) && !getBit(occupancies[both], g)) {
            //check if king, f and g squares are under attack
            if (!checkSquareAttacked(e, oppositeSide) &&
                !checkSquareAttacked(f, oppositeSide) &&
                !checkSquareAttacked(g, oppositeSide)) {
                move.setMove(e, g, sideToMove, king, 0, 0, 0, 0, 1);
                list.addMove(move);
            }
        }
    }

    //check if queenside castling is available
    if (castlingRights & queenside) {
        //check squares between king and rook are empty
        if (!getBit(occupancies[both], d) &&
            !getBit(occupancies[both], c) &&
            !getBit(occupancies[both], b)) {
            //check if king d and c squares are under attack
            if (!checkSquareAttacked(e, oppositeSide) &&
                !checkSquareAttacked(d, oppositeSide) &&
                !checkSquareAttacked(c, oppositeSide)) {
                move.setMove(e, c, sideToMove, king, 0, 0, 0, 0, 1);
                list.addMove(move);
            }
        }
    }
}

void Board::genKnightMoves(MoveList &list) {

    //define move
    Move move;

    //define initial and destination squares
    int initialSquare, destinationSquare;

    //initialise opposite side
    int oppositeSide = (sideToMove == white) ? black : white;

    //define piece bitboard and attacks bitboard
    bitboard bitboard, attacks;

    bitboard = pieces[sideToMove][knight];

    //loop over all pieces in the bitboard
    while (bitboard) {
        //set initial square
        initialSquare = getLeastSignificantBit(bitboard);

        //initialise knight attacks bitboard for possible destinations
        attacks = (knightAttackMasks[initialSquare] & ~(occupancies[sideToMove]));

        //loop over all destination squares
        while (attacks) {
            //initialise destination square
            destinationSquare = getLeastSignificantBit(attacks);

            //quiet move
            if (!getBit(occupancies[oppositeSide], destinationSquare)) {
                move.setMove(initialSquare, destinationSquare, sideToMove, knight, 0, 0, 0, 0, 0);
                list.addMove(move);
            }
            else {
                //handling captures
                move.setMove(initialSquare, destinationSquare, sideToMove, knight, 0, 1, 0, 0, 0);
                list.addMove(move);
            }
            //clear current knight move
            clearBit(attacks, destinationSquare);
        }
        //clear current knight attack
        clearBit(bitboard, initialSquare);
    }
}

void Board::genBishopMoves(MoveList &list) {

    //initialise move
    Move move;

    //define initial and destination squares
    int initialSquare, destinationSquare;

    //initialise opposite side
    int oppositeSide = (sideToMove == white) ? black : white;

    //define piece bitboard and attacks bitboard
    bitboard bitboard, attacks;

    bitboard = pieces[sideToMove][bishop];

    //loop over all pieces in the bitboard
    while (bitboard) {
        //initialise initial square
        initialSquare = getLeastSignificantBit(bitboard);

        //initialise bishop attacks bitboard for possible destinations
        attacks = (generateBishopAttack(initialSquare, occupancies[both]) &
                   ~(occupancies[sideToMove]));


        //loop over all destination squares
        while (attacks) {
            //initialise destination square
            destinationSquare = getLeastSignificantBit(attacks);

            //quiet move
            if (!getBit(occupancies[oppositeSide], destinationSquare)) {
                move.setMove(initialSquare, destinationSquare, sideToMove, bishop, 0, 0, 0, 0, 0);
                list.addMove(move);
            }
            else {
                //handling capture
                move.setMove(initialSquare, destinationSquare, sideToMove, bishop, 0, 1, 0, 0, 0);
                list.addMove(move);
            }
            //clear current bishop move
            clearBit(attacks, destinationSquare);
        }
        //clear current bishop piece
        clearBit(bitboard, initialSquare);
    }
}

void Board::genRookMoves(MoveList &list) {

    Move move;

    //define initial and destination squares
    int initialSquare, destinationSquare;

    //initialise opposite side
    int oppositeSide = (sideToMove == white) ? black : white;

    //define piece bitboard and attacks bitboard
    bitboard bitboard, attacks;

    bitboard = pieces[sideToMove][rook];

    //loop over all pieces in bitboard
    while (bitboard) {
        //initialise initial square
        initialSquare = getLeastSignificantBit(bitboard);

        //generate all possible rook moves
        attacks = (generateRookAttack(initialSquare, occupancies[both]) &
                   ~(occupancies[sideToMove]));


        //loop over all possible moves
        while (attacks) {
            //initialise destination square
            destinationSquare = getLeastSignificantBit(attacks);

            //quiet move
            if (!getBit(occupancies[oppositeSide], destinationSquare)) {
                move.setMove(initialSquare, destinationSquare, sideToMove, rook, 0, 0, 0, 0, 0);
                list.addMove(move);
            }
            else {
                //handling captures
                move.setMove(initialSquare, destinationSquare, sideToMove, rook, 0, 1, 0, 0, 0);
                list.addMove(move);
            }
            //clear current move
            clearBit(attacks, destinationSquare);
        }
        //clear current piece
        clearBit(bitboard, initialSquare);
    }
}

void Board::genQueenMoves(MoveList &list) {

    //initialise move
    Move move;

    //define initial and destination squares
    int initialSquare, destinationSquare;

    //initialise opposite side
    int oppositeSide = (sideToMove == white) ? black : white;

    //define piece bitboard and attacks bitboard
    bitboard bitboard, attacks;

    bitboard = pieces[sideToMove][queen];

    //loop over all queens
    while (bitboard) {
        //initialise initial square
        initialSquare = getLeastSignificantBit(bitboard);

        //generate all queen possible moves
        attacks = (generateQueenAttack(initialSquare, occupancies[both]) &
                   ~(occupancies[sideToMove]));


        //loop over all queen moves
        while (attacks) {
            //initialise destination square
            destinationSquare = getLeastSignificantBit(attacks);

            //quiet move
            if (!getBit(occupancies[oppositeSide], destinationSquare)) {
                move.setMove(initialSquare, destinationSquare, sideToMove, queen, 0, 0, 0, 0, 0);
                list.addMove(move);
            }
            else {
                //handling captures
                move.setMove(initialSquare, destinationSquare, sideToMove, queen, 0, 1, 0, 0, 0);
                list.addMove(move);
            }
            //clear current queen move
            clearBit(attacks, destinationSquare);
        }
        //clear current queen
        clearBit(bitboard, initialSquare);
    }
}

void Board::genKingMoves(MoveList &list) {

    //initialise move
    Move move;

    //define initial and destination squares
    int initialSquare, destinationSquare;

    //initialise opposite side
    int oppositeSide = (sideToMove == white) ? black : white;

    //define piece bitboard and attacks bitboard
    bitboard bitboard, attacks;

    bitboard = pieces[sideToMove][king];

    //loop over king bitboard
    while (bitboard) {
        //initialise initial square
        initialSquare = getLeastSignificantBit(bitboard);

        //initialise king moves
        attacks = (kingAttackMasks[initialSquare] & ~(occupancies[sideToMove]));

        //loop over king moves
        while (attacks) {
            //initialise destination square
            destinationSquare = getLeastSignificantBit(attacks);

            //quiet move
            if (!getBit(occupancies[oppositeSide], destinationSquare)) {
                move.setMove(initialSquare, destinationSquare, sideToMove, king, 0, 0, 0, 0, 0);
                list.addMove(move);
            }
            else {
                //handling captures
                move.setMove(initialSquare, destinationSquare, sideToMove, king, 0, 1, 0, 0, 0);
                list.addMove(move);
            }
            //clear current move
            clearBit(attacks, destinationSquare);
        }
        //clear king
        clearBit(bitboard, initialSquare);
    }
}

void Board::genWhitePawnMoves(MoveList &list) {

    //define move
    Move move;

    //define initial and destination squares
    int initialSquare, destinationSquare;

    //initiate enpassant value
    int enpass = enpassant;

    //define current piece bitboard, current piece attacks bitboard and enpass bitboard
    bitboard bitboard, attacks, enpassantPosition;

    bitboard = pieces[white][pawn];

    //loop over all pieces in the bitboard
    while (bitboard) {
        //assign the initial square to the square of the first pawn
        initialSquare = getLeastSignificantBit(bitboard);

        //destination square is 8 bits ahead of pawn square
        destinationSquare = initialSquare + 8;

        //destination square must be int value that is on the board and not occupied
        if (destinationSquare <= h8 && !getBit(occupancies[both], destinationSquare)) {
            //handling all promotion cases
            //make sure pawn is on second to last rank
            if (initialSquare >= a7 && initialSquare <= h7) {
                move.setMove(initialSquare, destinationSquare, white, pawn, queen, 0, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, white, pawn, rook, 0, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, white, pawn, bishop, 0, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, white, pawn, knight, 0, 0, 0, 0);
                list.addMove(move);
            }

            else {
                //pawn moving one square forward
                move.setMove(initialSquare, destinationSquare, white, pawn, 0, 0, 0, 0, 0);
                list.addMove(move);

                //pawn moving 2 squares forward, check whether pawn is on second rank and 4th rank is clear
                if ((initialSquare >= a2 && initialSquare <= h2)
                    && !getBit(occupancies[both], destinationSquare + 8)) {
                    move.setMove(initialSquare, destinationSquare + 8, white, pawn, 0, 0, 1, 0, 0);
                    list.addMove(move);
                }
            }
        }
        //handling pawn captures
        //initialise pawn attacks bitboard
        attacks = pawnAttackMasks[white][initialSquare] & occupancies[black];

        //generate pawn attacks
        while (attacks) {
            //initialise destination square
            destinationSquare = getLeastSignificantBit(attacks);

            //handling pawn capture promotions
            //make sure pawn is on second to last rank
            if (initialSquare >= a7 && initialSquare <= h7) {
                move.setMove(initialSquare, destinationSquare, white, pawn, queen, 1, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, white, pawn, rook, 1, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, white, pawn, bishop, 1, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, white, pawn, knight, 1, 0, 0, 0);
                list.addMove(move);
            }

            else {
                //regular pawn capture
                move.setMove(initialSquare, destinationSquare, white, pawn, 0, 1, 0, 0, 0);
                list.addMove(move);
            }

            //clear current pawn attack
            clearBit(attacks, destinationSquare);
        }

        //handling enpassant
        if (enpass != noSquare) {
            //generate pawn attack for the enpassant square
            enpassantPosition = pawnAttackMasks[white][initialSquare] & (1ULL << enpass);

            //if enpassant is available
            if (enpassantPosition) {
                //initialise the enpassant destination square
                int enpassantDestination = getLeastSignificantBit(enpassantPosition);
                move.setMove(initialSquare, enpassantDestination, white, pawn, 0, 1, 0, 1, 0);
                list.addMove(move);
            }
        }
        //clear the current pawn from board and move onto next
        clearBit(bitboard, initialSquare);
    }
}

void Board::genBlackPawnMoves(MoveList &list) {

    //define move
    Move move;

    //define initial and destination squares
    int initialSquare, destinationSquare;

    //initiate enpass value
    int enpass = enpassant;

    //define current piece bitboard, it's attacks and enpassant bitboard
    bitboard bitboard, attacks, enpassantPosition;

    bitboard = pieces[black][pawn];

    while (bitboard) {
        //assign the initial square to the square of the first pawn
        initialSquare = getLeastSignificantBit(bitboard);

        //destination square is 8 bits ahead of pawn square
        destinationSquare = initialSquare - 8;

        //destination square must be int value that is on the board and not occupied
        if (destinationSquare >= a1 && !getBit(occupancies[both], destinationSquare)) {
            //handling all promotion cases
            //make sure pawn is on second to last rank
            if (initialSquare >= a2 && initialSquare <= h2) {
                move.setMove(initialSquare, destinationSquare, black, pawn, queen, 0, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, black, pawn, rook, 0, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, black, pawn, bishop, 0, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, black, pawn, knight, 0, 0, 0, 0);
                list.addMove(move);
            }

            else {
                //pawn moving one square forward
                move.setMove(initialSquare, destinationSquare, black, pawn, 0, 0, 0, 0, 0);
                list.addMove(move);

                //pawn moving 2 squares forward, check whether pawn is on second rank and 4th rank is clear
                if ((initialSquare >= a7 && initialSquare <= h7)
                    && !getBit(occupancies[both], destinationSquare - 8)) {
                    move.setMove(initialSquare, destinationSquare - 8, black, pawn, 0, 0, 1, 0, 0);
                    list.addMove(move);
                }
            }
        }
        //handling pawn captures
        //initialise pawn attacks bitboard
        attacks = pawnAttackMasks[black][initialSquare] & occupancies[white];

        //generate pawn attacks
        while (attacks) {
            //initialise destination square
            destinationSquare = getLeastSignificantBit(attacks);

            //handling pawn capture promotions
            //make sure pawn is on second to last rank
            if (initialSquare >= a2 && initialSquare <= h2) {
                move.setMove(initialSquare, destinationSquare, black, pawn, queen, 1, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, black, pawn, rook, 1, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, black, pawn, bishop, 1, 0, 0, 0);
                list.addMove(move);
                move.setMove(initialSquare, destinationSquare, black, pawn, knight, 1, 0, 0, 0);
                list.addMove(move);
            }

            else {
                //pawn capture
                move.setMove(initialSquare, destinationSquare, black, pawn, 0, 1, 0, 0, 0);
                list.addMove(move);
            }

            //clear current pawn attack
            clearBit(attacks, destinationSquare);
        }

        //handling enpass
        if (enpass != noSquare) {
            //generate pawn attack for the enpass square
            enpassantPosition = pawnAttackMasks[black][initialSquare] & (1ULL << enpass);

            //if enpass is available
            if (enpassantPosition) {
                //initiate enpass capture
                int enpassantDestination = getLeastSignificantBit(enpassantPosition);
                move.setMove(initialSquare, enpassantDestination, black, pawn, 0, 1, 0, 1, 0);
                list.addMove(move);
            }
        }
        //clear the current pawn from board and move onto next
        clearBit(bitboard, initialSquare);
    }
}

void Board::genMoves(MoveList &list) {
    genKnightMoves(list);
    genBishopMoves(list);
    genRookMoves(list);
    genQueenMoves(list);
    genKingMoves(list);
    genCastlingMoves(list);
    sideToMove == white ? genWhitePawnMoves(list) : genBlackPawnMoves(list);
}

int Board::evaluate() {
    int whiteScore = 0;
    int blackScore = 0;

    whiteScore += pieceScoreWhite(pieces[white][pawn], pieces[white][knight], pieces[white][bishop],
                            pieces[white][rook], pieces[white][queen], pieces[white][king]);

    blackScore += pieceScoreBlack(pieces[black][pawn], pieces[black][knight], pieces[black][bishop],
                            pieces[black][rook], pieces[black][queen], pieces[black][king]);

    return (sideToMove == white) ? (whiteScore - blackScore) : (blackScore - whiteScore);

}



void Board::perftTest(int depth) {

    MoveList moves;

    //leaf node visited
    if (depth == 0) {
        nodesVisited++;
        return;
    }

    genMoves(moves);
    for (int i = 0; i < moves.getSize(); i++) {

        Board copy = Board(*this);

        if (makeMove(moves.getMoveIndex(i)) == 0) {
            continue;
        }

        perftTest(depth - 1);

        restore(copy);
    }
}



int Board::negamaxSearch(int depth, int alpha, int beta, Move &chosenMove) {
    //legal move count, if no legal moves then checkmate/stalemate
    int legalMoves = 0;

    //flag for the board position key being in the positionKeys set, used for repetitions
    bool repeat = false;

    //initialise movelist
    MoveList moves;

    //initialise best local move variable
    Move localChosenMove;

    //base case
    if (depth == 0) {
        return evaluate();
    }

    genMoves(moves);
    moves.sortMoveList();

    for (int i = 0; i < moves.getSize(); i++) {

        //copy board state
        Board copy = Board(*this);

        //check if move is legal
        if (makeMove(moves.getMoveIndex(i)) == 0) {
            //skip to next move
            continue;
        }

        legalMoves++;

        //check for repetition
        if (positionKeys.count(positionKey) == 1) {
            repeat = true;
        }

        //add current position identifier to the set
        positionKeys.insert(positionKey);

        //score current move
        int score = -negamaxSearch(depth - 1, -beta, -alpha, chosenMove);

        //set draw score for repetition, and erase position key from set if wasn't already contained within
        if(repeat)
            score = 0;
        else
            positionKeys.erase(positionKey);

        repeat = false;


        //restore board state
        restore(copy);

        //pruning
        if (score >= beta) {
            chosenMove = moves.getMoveIndex(i);
            return beta;
        }

        //update alpha
        if (score > alpha) {
            alpha = score;
            localChosenMove = moves.getMoveIndex(i);
        }
    }
    //set draw score for 50move rule
    if(fiftyMove >= 100) {
        return 0;
    }

    //go for checkmate
    if(legalMoves == 0) {
        int kingSquare = getLeastSignificantBit(pieces[sideToMove][king]);
        if(checkSquareAttacked(kingSquare, !sideToMove))
            return -49000 - depth;
        else
            return 0;
    }
    chosenMove = localChosenMove;
    //return the best score found
    return alpha;
}

Move Board::idNegamax(int maxDepth, int time) {
    Move bestMove;
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::milliseconds(time);

    for(int i = 5; i <= maxDepth; i++) {
        //Check if time is up
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime >= endTime)
            break;
        //std::cout<<i<<"\n";
        negamaxSearch(i, -50000, 50000, bestMove);
    }
    return bestMove;
}

void Board::engine(int depth, int time) {
    Move compMove;
    compMove = idNegamax(depth, time);

    if (compMove.getInitialSquare() == compMove.getDestinationSquare()){
        if (checkSquareAttacked(getLeastSignificantBit(pieces[sideToMove][king]), sideToMove^1))
            int stop = 0;


    } else
        playMove(compMove);
}

bitboard Board::generatePosKey() {
    bitboard key;

    bitboard occupied;

    for (int side = 0; side < 2; side++) {
        for (int piece = 0; piece < 6; piece++) {
            occupied = pieces[side][piece];
            while(occupied){
                int square = getLeastSignificantBit(occupied);
                key ^= zorbistPieceKeys[side][piece][square];
                clearBit(occupied, square);
            }

        }
    }
    if(sideToMove == black)
        key ^= sideKey;
    return key;
}

int Board::countPositions() {
    int count = 0;
    for (bitboard num: positions) {
        if (num == positionKey) {
            count++;
        }
    }
    return count;
}

