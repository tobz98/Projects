#include <iostream>
#include "AttackMasks.h"
#include "Bitboard.h"
#include "Board.h"
#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <thread>

//images https://opengameart.org/content/chess-pieces-and-board-squares

//initiate values
Board board;

MoveList moves;

int windowWidth = 600;
int windowHeight = 600;

GLuint textureIDs[2][6];

//variables used for the game loop
bool playVsComp = true;
bool computerTurn = true;
bool playerTurn = true;
bool compBattle = false;
int playWhite = 1;
int playBlack = 0;

//global variables to store the selected square and whether a piece is selected, used in mouse click function
int startSquare = -1;
bool pieceSelected = false;


GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (image) {
        int format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        stbi_image_free(image);
    } else {
        std::cout << "Failed to load texture" << stbi_failure_reason() << std::endl;
        return 0;
    }

    return textureID;
}

void loadPieceTextures() {
    textureIDs[white][pawn] = loadTexture(R"(256h\white_pawn.png)");
    textureIDs[white][knight] = loadTexture(R"(256h\white_knight.png)");
    textureIDs[white][bishop] = loadTexture(R"(256h\white_bishop.png)");
    textureIDs[white][rook] = loadTexture(R"(256h\white_rook.png)");
    textureIDs[white][queen] = loadTexture(R"(256h\white_queen.png)");
    textureIDs[white][king] = loadTexture(R"(256h\white_king.png)");
    textureIDs[black][pawn] = loadTexture(R"(256h\black_pawn.png)");
    textureIDs[black][knight] = loadTexture(R"(256h\black_knight.png)");
    textureIDs[black][bishop] = loadTexture(R"(256h\black_bishop.png)");
    textureIDs[black][rook] = loadTexture(R"(256h\black_rook.png)");
    textureIDs[black][queen] = loadTexture(R"(256h\black_queen.png)");
    textureIDs[black][king] = loadTexture(R"(256h\black_king.png)");
}

void drawSquare(float x, float y, float sideSize, bool isBlack) {
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    //set colour
    if (isBlack)
        glColor4f(179/255.0 ,134/255.0, 100/255.0, 0);
    else
        glColor4f(242/255.0, 219/255.0, 179/255.0, 0);


    glVertex2f(x, y);

    glVertex2f(x + sideSize, y);

    glVertex2f(x + sideSize, y + sideSize);

    glVertex2f(x, y + sideSize);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void drawSquareTexture(float x, float y, float sideSize, GLuint textureID, bool isBlack) {
    //coordinates for last move played
    int square = board.getLastMove().getDestinationSquare();
    int rank = square/8;
    int file = square%8;
    float xMove = file * sideSize - 1.0f;
    float yMove = rank * sideSize - 1.0f;

    //coordinates for mouse selected piece
    int pieceRank = startSquare/8;
    int pieceFile = startSquare%8;
    float pieceXMove = pieceFile * sideSize - 1.0f;
    float pieceYMove = pieceRank * sideSize - 1.0f;

    if (isBlack) {
        glColor3f(179/255.0, 134/255.0, 100/255.0);
    }
    else {
        glColor3f(242/255.0, 219/255.0, 179/255.0);
    }
    if(board.getLastMove().getDestinationSquare() != board.getLastMove().getInitialSquare()) {
        if(xMove == x && yMove == y) {
            glColor3f(255/255.0, 168/255.0, 86/255.0);
        }
    }
    if(startSquare != -1) {
        if(pieceXMove == x && pieceYMove == y) {
            glColor3f(255/255.0, 200/255.0, 120/255.0);
        }
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);

    //draw texture square
    glTexCoord2f(0.0f, 0.0f); // Bottom-left corner of the texture
    glVertex2f(x, y);

    glTexCoord2f(1.0f, 0.0f); // Bottom-right corner of the texture
    glVertex2f(x + sideSize, y);

    glTexCoord2f(1.0f, 1.0f); // Top-right corner of the texture
    glVertex2f(x + sideSize, y + sideSize);

    glTexCoord2f(0.0f, 1.0f); // Top-left corner of the texture
    glVertex2f(x, y + sideSize);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawPieces(Board chessBoard) {
    float sideSize = 1.0f / 4.0f;
    bool isBlack = true;
    //loop over all squares
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            //set x,y coordinates of square
            float x = file * sideSize - 1.0f;
            float y = rank * sideSize - 1.0f;
            bool foundPiece = false; //flag to track if a piece was found on this square

            //loop over pieces
            for (int colour = 0; colour < 2; colour++) {
                for (int piece = 0; piece < 6; piece++) {
                    bitboard pieceBB = chessBoard.getPieceBitboard(colour, piece);

                    if (getBit(pieceBB, rank * 8 + file)) {
                        //drawSquare(x, y, sideSize, isBlack);
                        drawSquareTexture(x, y, sideSize, textureIDs[colour][piece], isBlack);
                        foundPiece = true; //set flag to true if a piece is found
                        break;
                    }
                } if (foundPiece) //if a piece is found, no need to continue searching
                    break;
            }

            isBlack = !isBlack; // Toggle the colour for the next square
        }
        isBlack = !isBlack; // Toggle the colour for the next rank
    }
}

void drawChessBoard() {
    float sideSize = 1.0f / 4.0f;
    bool isBlack = true;
    //loop over all squares
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            //set x,y coordinates of square
            float x = file * sideSize - 1.0f;
            float y = rank * sideSize - 1.0f;
            drawSquare(x, y, sideSize, isBlack);

            isBlack = !isBlack; // Toggle the colour for the next square
        }
        isBlack = !isBlack; // Toggle the colour for the next rank
    }
}

void makeComputerMove() {
    board.engine(5, 1000);
    if(playVsComp)
        playerTurn = true;
    glutPostRedisplay();
    if(board.countPositions() == 3) {
        std::cout << "stalemate by repetition";
        playerTurn = false;
    }
}

void mouse(int button, int state, int x, int y) {
    if(playerTurn){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            //convert mouse coordinates to square number
            int rank = (7 - (y / (windowHeight / 8))) * 8;
            int file = x / (windowWidth / 8);
            int square = rank + file;

            //generate moves in position
            board.genMoves(moves);
            board.genLegalMoves(moves);
            // If a piece is already selected, move it to the clicked square
            if (pieceSelected) {

                Move move = moves.getMove(startSquare, square);

                if(move.getInitialSquare() != move.getDestinationSquare()) {

                    //promotion
                    if(move.getPromotionPiece() != 0) {
                        move = moves.getMovePromo(startSquare, square, queen);
                    }

                    //make move if it is in list
                    board.playMove(move);

                    //if playing vs computer = true, set player turn off
                    if(playVsComp){
                        playerTurn = false;
                        computerTurn = true;
                    }
                    //update display after move
                    glutPostRedisplay();

                }
                pieceSelected = false;
                startSquare = -1;
            }
            else {
                if (getBit(board.getAllPieces(), square)) {
                    //set selected square
                    startSquare = square;
                    pieceSelected = true;
                }
            }
            //clear move list
            moves.eraseMoves();
        }
    }
}

// Display function
void display() {
    loadPieceTextures();

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draw board
    drawChessBoard();
    drawPieces(board);

    for(int colour = 0; colour < 2; colour++)
        for(int piece = 0; piece < 6; piece++) {
        glDeleteTextures(1, &textureIDs[colour][piece]);
    }

    glFlush();
}

void gameLoop() {
    //check if playing vs computer or not
    if(playVsComp)
        //if comp turn make computer move
        if(computerTurn) {
            makeComputerMove();
            computerTurn = false;
            //if comp vs comp, set comp turn back on
            if(compBattle)
                computerTurn = true;
        }
    if(board.countPositions() == 3) {
        std::cout << "stalemate by repetition";
        playerTurn = false;
    }
    if(board.getFiftyMove() >= 100) {
        std::cout << "50 move rule, draw";
        playerTurn = false;
    }

    //update display
    glutPostRedisplay();
}

//init function
void init() {
    glClearColor(0.5, 0.5, 0.5, 0.5);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

//reshape function
void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0 * width / height, 1.0 * width / height, -1.0, 1.0, -1.0, 1.0);
}

void gameMode(){
    int gameMode = 0;
    int sideChoice = 0;

    std::cout << "Would you like to play vs computer (1), play vs player (2) or watch computer vs computer (3)?\n";
    std::cin >> gameMode;

    while((gameMode != 1) && (gameMode != 2) && (gameMode!= 3)) {
        std::cout << "Please enter a valid option: 1, 2, or 3.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> gameMode;
    }

    if(gameMode == 1) {
        std::cout << "Would you like to play as white (0) or as black (1)?\n";
        std::cin >> sideChoice;

        while((sideChoice != 0) && (sideChoice != 1)) {
            std::cout << "Please enter a valid option: 0 or 1.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> sideChoice;
        }
    }

    //choose if vs computer or not
    playVsComp = (gameMode != 2);
    //choose side to move
    computerTurn = sideChoice;

    if(gameMode == 3) {
        playerTurn = false;
        compBattle = true;
        computerTurn = playWhite;
    }


}

int main(int argc, char** argv) {

    createPawnAttacks();
    createKnightAttacks();
    createKingAttacks();
    initZorbist();

    gameMode();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Chessboard");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutIdleFunc(gameLoop);

    glutMainLoop();
}
