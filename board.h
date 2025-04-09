#ifndef BOARD_H
#define BOARD_H
#include "pawn.h"
#include "king.h"
#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "knight.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "piece.h"

struct Board {
    SDL_Texture* pieceTextures[12]; // Mảng texture cho quân cờ
    std::vector<Piece*> pieces; // Danh sách quân cờ
    Piece* selectedPiece = nullptr; // Quân cờ đang được chọn

    void draw(SDL_Renderer* renderer);
    void init(SDL_Renderer* renderer);
    void handleMouseClick(int x, int y);
    void updateSize(int newWidth, int newHeight);
};



#endif

