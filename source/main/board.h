#ifndef BOARD_H
#define BOARD_H
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Piece.h"

#include "graphic.h"
struct Board {

    SDL_Texture* pieceTextures[12];


    void init(SDL_Renderer* renderer);



    void draw(SDL_Renderer* renderer);



    std::vector<Piece*> pieces;
    Piece* selectedPiece = nullptr;
    Piece* getPieceAt(int row, int col);
    graphic& gfx;
    void reset(int timeLimit);
    Piece* showPromotionMenu(int row, int col, bool turnIsWhite, SDL_Renderer* renderer);
    Board(graphic& g) : gfx(g) {} // Constructor nhận tham chiếu graphic


    void handleMouseClick(int x, int y, SDL_Renderer* renderer);

    bool gameOver = false;
    bool isInCheck(bool white);
    bool isCheckmate(bool white);
    Piece* getKing(bool white);
    bool kingCanMove(Piece* king, std::vector<Piece*>& pieces);
    Piece* checkingPiece = nullptr;
    std::vector<std::pair<int, int>> validMoves;
    int oldRow;
    int oldCol;
    bool isSquareUnderAttack(int row, int col, bool whiteKing);
    bool wasInCheck = false;

    bool turnIsWhite = true;
    Uint32 whiteStartTime = 0;
    Uint32 blackStartTime = 0;
    int whiteTimeLeft = 300;
    int blackTimeLeft = 300;

    bool isKingInCheck(bool isWhite);


    // draw check function
    bool isStalemate();
    bool isInsufficientMaterial();
};

#endif

