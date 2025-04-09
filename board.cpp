#include "board.h"
#include <iostream>
#include <algorithm>

void Board::draw(SDL_Renderer* renderer) {
    int tileSize = 75;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 107, 142, 35, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 227, 218, 201, 255);
            }

            SDL_Rect rect = { col * tileSize, row * tileSize, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &rect);
        }
    }


    for (Piece* piece : pieces) {
        SDL_Rect pieceRect = { piece->getCol() * tileSize, piece->getRow() * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, pieceTextures[piece->getType()], NULL, &pieceRect);
    }
}


SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cerr << "Không thể load ảnh! Lỗi: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}


void Board::init(SDL_Renderer* renderer) {

    pieceTextures[0] = loadTexture("E:/ChessGame/img/king1.png", renderer);
    pieceTextures[1] = loadTexture("E:/ChessGame/img/queen1.png", renderer);
    pieceTextures[2] = loadTexture("E:/ChessGame/img/rook1.png", renderer);
    pieceTextures[3] = loadTexture("E:/ChessGame/img/bishop1.png", renderer);
    pieceTextures[4] = loadTexture("E:/ChessGame/img/knight1.png", renderer);
    pieceTextures[5] = loadTexture("E:/ChessGame/img/pawn1.png", renderer);

    pieceTextures[6] = loadTexture("E:/ChessGame/img/king2.png", renderer);
    pieceTextures[7] = loadTexture("E:/ChessGame/img/queen2.png", renderer);
    pieceTextures[8] = loadTexture("E:/ChessGame/img/rook2.png", renderer);
    pieceTextures[9] = loadTexture("E:/ChessGame/img/bishop2.png", renderer);
    pieceTextures[10] = loadTexture("E:/ChessGame/img/knight2.png", renderer);
    pieceTextures[11] = loadTexture("E:/ChessGame/img/pawn2.png", renderer);


    pieces.push_back(new Piece(0, 4, true, 0)); // Vua trắng
    pieces.push_back(new Piece(0, 3, true, 1)); // Hậu trắng
    pieces.push_back(new Piece(0, 0,true, 2)); // Xe trắng
    pieces.push_back(new Piece(0, 7,true, 2)); // Xe trắng
    pieces.push_back(new Piece(0, 2,true, 3)); // Tượng trắng
    pieces.push_back(new Piece(0, 5,true, 3)); // Tượng trắng
    pieces.push_back(new Piece(0, 1,true,4)); // Mã trắng
    pieces.push_back(new Piece(0, 6,true,4)); // Mã trắng

    // Tốt trắng
    for (int i = 0; i < 8; i++) {
        pieces.push_back(new Piece(1, i, 1, 5));
    }

    // Khởi tạo quân cờ đen
    pieces.push_back(new Piece(7, 4,false,6)); // Vua đen
    pieces.push_back(new Piece(7, 3,false,7)); // Hậu đen
    pieces.push_back(new Piece(7, 0,false,8)); // Xe đen
    pieces.push_back(new Piece(7, 7,false, 8)); // Xe đen
    pieces.push_back(new Piece(7, 2,false,9)); // Tượng đen
    pieces.push_back(new Piece(7, 5,false, 9)); // Tượng đen
    pieces.push_back(new Piece(7, 1,false, 10)); // Mã đen
    pieces.push_back(new Piece(7, 6,false,10)); // Mã đen

    // Tốt đen
    for (int i = 0; i < 8; i++) {
        pieces.push_back(new Piece(6, i, 0, 11));
    }
}


bool turnIsWhite = true;  // Quân trắng đi trước

void Board::handleMouseClick(int x, int y) {
    int row = y / 75;
    int col = x / 75;
    std::cout << "Click at (" << row << ", " << col << ")\n";

    if (selectedPiece == nullptr) {
        std::cout << "No piece selected, trying to select...\n";
        for (Piece* piece : pieces) {
            if (piece->getRow() == row && piece->getCol() == col && piece->isWhite == turnIsWhite) {
                selectedPiece = piece;
                std::cout << "Selected piece at (" << row << ", " << col << ")\n";
                std::cout << "Piece type: " << piece->getType() << ", isWhite: " << piece->isWhite << "\n";
                return;
            }
        }
        std::cout << "No valid piece found at this position\n";
    } else {
        std::cout << "Attempting to move piece from (" << selectedPiece->getRow()
                  << ", " << selectedPiece->getCol() << ") to (" << row << ", " << col << ")\n";


        bool canMove = true;

        // Kiểm tra xem ô đích có quân cùng màu không
        for (Piece* piece : pieces) {
            if (piece->getRow() == row && piece->getCol() == col && piece->isWhite == selectedPiece->isWhite) {
                canMove = false;
                std::cout << "Can't capture your own piece\n";
                break;
            }
        }

        if (canMove) {
            std::cout << "Move executed (no validation)\n";
            selectedPiece->move(row, col);
            turnIsWhite = !turnIsWhite;  // Đổi lượt
        } else {
            std::cout << "Move blocked by friendly piece\n";
        }

        selectedPiece = nullptr;
    }
}
