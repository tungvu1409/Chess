#include "board.h"
#include "queen.h" // Đảm bảo bạn có file queen.h
#include <iostream>
#include <algorithm>

bool turnIsWhite = true;  // Trắng đi trước

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

        SDL_RenderCopy(renderer, pieceTextures[piece->getType() + (piece->isWhite ? 0 : 6)], NULL, &pieceRect);
    }

}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cerr << "Không thể load ảnh! Lỗi: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
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

    // Trắng
    pieces.push_back(new King(0, 4, true));
    pieces.push_back(new Queen(0, 3, true));
    pieces.push_back(new Rook(0, 0, true));
    pieces.push_back(new Rook(0, 7, true));
    pieces.push_back(new Bishop(0, 2, true));
    pieces.push_back(new Bishop(0, 5, true));
    pieces.push_back(new Knight(0, 1, true));
    pieces.push_back(new Knight(0, 6, true));
    for (int i = 0; i < 8; i++)
        pieces.push_back(new Pawn(1, i, true));

    // Đen
    pieces.push_back(new King(7, 4, false));
    pieces.push_back(new Queen(7, 3, false));
    pieces.push_back(new Rook(7, 0, false));
    pieces.push_back(new Rook(7, 7, false));
    pieces.push_back(new Bishop(7, 2, false));
    pieces.push_back(new Bishop(7, 5, false));
    pieces.push_back(new Knight(7, 1, false));
    pieces.push_back(new Knight(7, 6, false));
    for (int i = 0; i < 8; i++)
        pieces.push_back(new Pawn(6, i, false));

    }

void Board::handleMouseClick(int x, int y) {
    if (gameOver) return;

    int row = y / 75;
    int col = x / 75;
    std::cout << "Lượt hiện tại: " << (turnIsWhite ? "Trắng" : "Đen") << "\n";


    if (!selectedPiece) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == row && piece->getCol() == col && piece->isWhite == turnIsWhite) {
                selectedPiece = piece;
                std::cout << "Đã chọn quân ở (" << row << ", " << col << ") - type: " << piece->getType() << "\n";
                return;
            }
        }
    } else {
        // Kiểm tra xem nước đi hợp lệ không
        if (selectedPiece->isValidMove(row, col, pieces)) {
            std::cout << "Di chuyển quân từ (" << selectedPiece->getRow() << ", " << selectedPiece->getCol() << ") đến (" << row << ", " << col << ")\n";

            // Ăn quân đối phương nếu có
            for (auto it = pieces.begin(); it != pieces.end(); ++it) {
                Piece* piece = *it;
                if (piece->getRow() == row && piece->getCol() == col && piece->isWhite != selectedPiece->isWhite) {
                    std::cout << "Ăn quân đối phương ở (" << row << ", " << col << ")\n";
                    delete piece;
                    pieces.erase(it);
                    break;
                }
            }

            // Di chuyển quân
            selectedPiece->move(row, col);
            std::cout << "Đã di chuyển quân đến (" << row << ", " << col << ")\n";

            // Kiểm tra chiếu / chiếu hết
            if (isInCheck(!turnIsWhite)) {
                std::cout << "Check!\n";
            }

            if (isCheckmate(!turnIsWhite)) {
                std::cout << "Checkmate! ";
                std::cout << (turnIsWhite ? "White wins!\n" : "Black wins!\n");
                gameOver = true;
            }

            turnIsWhite = !turnIsWhite;
        }

        selectedPiece = nullptr;  // Hủy chọn dù đi được hay không
    }
}



bool Board::isInCheck(bool whiteKing) {
    Piece* king = nullptr;
    for (Piece* p : pieces) {
        if (p->getType() == 0 && p->isWhite == whiteKing) {
            king = p;
            break;
        }
    }

    if (!king) return false;

    int kingRow = king->getRow();
    int kingCol = king->getCol();

    for (Piece* p : pieces) {
        if (p->isWhite != whiteKing) {
            if (p->canMoveTo(kingRow, kingCol, pieces)) {
                return true;
            }
        }
    }

    return false;
}

bool Board::isCheckmate(bool whiteKing) {
    if (!isInCheck(whiteKing)) return false;

    for (Piece* p : pieces) {
        if (p->isWhite == whiteKing) {
            auto moves = p->getAllPossibleMoves(pieces);
            for (auto& move : moves) {
                int oldRow = p->getRow(), oldCol = p->getCol();
                Piece* captured = getPieceAt(move.first, move.second);

                // Nếu có quân bị "ăn", tạm thời xóa ra khỏi mảng (nhưng không delete)
                bool removed = false;
                if (captured) {
                    pieces.erase(std::remove(pieces.begin(), pieces.end(), captured), pieces.end());
                    removed = true;
                }

                p->move(move.first, move.second);
                bool stillInCheck = isInCheck(whiteKing);
                p->move(oldRow, oldCol);

                if (removed) {
                    pieces.push_back(captured);  // Trả lại quân bị ăn
                }

                if (!stillInCheck) return false;
            }
        }
    }

    return true;
}

Piece* Board::getPieceAt(int row, int col) {
    for (Piece* piece : pieces) {
        if (piece->getRow() == row && piece->getCol() == col) {
            return piece;
        }
    }
    return nullptr;
}

