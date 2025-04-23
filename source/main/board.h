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
    SDL_Texture* pieceTextures[12]; // Mảng texture cho quân cờ
    std::vector<Piece*> pieces; // Danh sách quân cờ
    Piece* selectedPiece = nullptr; // Quân cờ đang được chọn
    Piece* getPieceAt(int row, int col);
    graphic& gfx;  // Sử dụng tham chiếu đến graphic


    Board(graphic& g) : gfx(g) {} // Constructor nhận tham chiếu graphic
    void draw(SDL_Renderer* renderer);
    void init(SDL_Renderer* renderer);
    void handleMouseClick(int x, int y);
    void updateSize(int newWidth, int newHeight);
    bool gameOver = false;
    bool isInCheck(bool white);
    bool isCheckmate(bool white);
    void displayCheckWarning(SDL_Renderer* renderer);
    // Kiểm tra xem ô có nằm trong bàn cờ không
    bool isInsideBoard(int row, int col);
    // Hàm tìm quân vua của một bên (trắng hoặc đen)
    Piece* getKing(bool white);
    // Kiểm tra xem quân vua có thể di chuyển không
    bool kingCanMove(Piece* king, std::vector<Piece*>& pieces);
    Piece* checkingPiece = nullptr;  // Quân cờ đang chiếu (nếu có)
    std::vector<std::pair<int, int>> validMoves;
    void highlightCheckSquares(const std::vector<std::pair<int, int>>& squares);
    void drawWarningCircle(int row, int col);

    //Hàm để lấy các ô vua bị chiếu
    std::vector<std::pair<int, int>> getCheckSquares(bool whiteKing);

    bool wasInCheck = false;
    bool turnIsWhite = true;
    Uint32 whiteStartTime = 0;
    Uint32 blackStartTime = 0;
    int whiteTimeLeft = 300;
    int blackTimeLeft = 300;



};



#endif

