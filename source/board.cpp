#include "board.h"
#include <iostream>
#include <algorithm>

bool turnIsWhite = true;

void Board::draw(SDL_Renderer* renderer) {
    int tileSize = 75;

    // Vẽ bàn cờ
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

    // Vẽ các dấu chấm gợi ý nước đi hợp lệ
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180); // màu trắng mờ
    for (const auto& move : validMoves) {
        int centerX = move.second * tileSize + tileSize / 2;
        int centerY = move.first * tileSize + tileSize / 2;

        SDL_Rect dot = {
            centerX - 6,
            centerY - 6,
            12,
            12
        };
        SDL_RenderFillRect(renderer, &dot);
    }

    // Vẽ các quân cờ
    for (Piece* piece : pieces) {
        SDL_Rect pieceRect = {
            piece->getCol() * tileSize,
            piece->getRow() * tileSize,
            tileSize,
            tileSize
        };

        SDL_RenderCopy(renderer, pieceTextures[piece->getType() + (piece->isWhite ? 0 : 6)], NULL, &pieceRect);
    }


    SDL_RenderPresent(renderer);
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

    // Check for piece selection
    if (!selectedPiece) {
        // Iterate through pieces to select the clicked one
        for (Piece* piece : pieces) {
            if (piece->getRow() == row && piece->getCol() == col && piece->isWhite == turnIsWhite) {
                selectedPiece = piece;

                // Find valid moves for the selected piece
                validMoves.clear();
                for (int r = 0; r < 8; ++r) {
                    for (int c = 0; c < 8; ++c) {
                        if (piece->isValidMove(r, c, pieces)) {
                            validMoves.push_back({r, c});
                        }
                    }
                }

                return;
            }
        }
    } else {
        // Check if the move is valid for the selected piece
        if (selectedPiece->isValidMove(row, col, pieces)) {
            // Handle capturing an opponent's piece
            for (auto it = pieces.begin(); it != pieces.end(); ++it) {
                Piece* piece = *it;
                if (piece->getRow() == row && piece->getCol() == col && piece->isWhite != selectedPiece->isWhite) {
                    std::cout << "Captured opponent's piece at (" << row << ", " << col << ")\n";
                    if (piece->getType() == KING) {
                        std::cout << (turnIsWhite ? "White wins!" : "Black wins!") << std::endl;
                        gameOver = true;  // End the game
                    }
                    delete piece;
                    pieces.erase(it);
                    break;
                }
            }

            // Move the selected piece
            selectedPiece->move(row, col);

            gfx.playMoveSound(); // Gọi âm thanh di chuyển

            // Check if the king is in check after the move
            bool currentlyInCheck = isInCheck(turnIsWhite);
            if (currentlyInCheck && !wasInCheck) {
                std::cout << "Check! The king is in check!" << std::endl;
            }
            wasInCheck = currentlyInCheck;

            // Undo the move if the king is in check
            if (currentlyInCheck) {
                std::cout << "Check! The king is in check!" << std::endl;
                selectedPiece->move(selectedPiece->getRow(), selectedPiece->getCol());
            }

            // Check for checkmate
            if (isCheckmate(turnIsWhite)) {
                std::cout << "Checkmate! ";
                std::cout << (turnIsWhite ? "White wins!" : "Black wins!") << std::endl;
                gameOver = true;  // End the game
            }



            // Chuyển lượt
            turnIsWhite = !turnIsWhite;


        }
        selectedPiece = nullptr;
        validMoves.clear();
    }
}







bool Board::isInCheck(bool whiteKing) {
    if (gameOver) return false;  // Nếu game đã kết thúc, không kiểm tra chiếu

    Piece* king = nullptr;
    for (Piece* p : pieces) {
        if (p->getType() == KING && p->isWhite == whiteKing) {
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
Piece* Board::getKing(bool white) {
    for (Piece* p : pieces) {
        if (p->getType() == KING && p->isWhite == white) {
            return p;
        }
    }
    return nullptr;
}
bool Board::isInsideBoard(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool Board::kingCanMove(Piece* king, std::vector<Piece*>& pieces) {
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1},  {1, 0}, {1, 1}
    };

    int oldRow = king->getRow();
    int oldCol = king->getCol();

    for (auto& dir : directions) {
        int newRow = oldRow + dir[0];
        int newCol = oldCol + dir[1];

        if (!isInsideBoard(newRow, newCol)) continue;

        Piece* target = getPieceAt(newRow, newCol);
        if (target && target->isWhite == king->isWhite) continue; // Không đi vào ô của quân mình

        // Thử di chuyển vua
        Piece* captured = nullptr;
        if (target) {
            captured = target;
            pieces.erase(std::remove(pieces.begin(), pieces.end(), target), pieces.end());
        }

        king->move(newRow, newCol);

        // Kiểm tra lại nếu quân vua bị chiếu sau khi di chuyển
        if (isInCheck(king->isWhite)) {
            king->move(oldRow, oldCol); // Quay lại vị trí cũ
            if (captured) pieces.push_back(captured);
            continue; // Nếu quân vua bị chiếu, không chấp nhận di chuyển này
        }

        king->move(oldRow, oldCol); // Quay lại vị trí cũ

        if (captured) pieces.push_back(captured);

        return true; // Quân vua có thể di chuyển ra khỏi chiếu
    }

    return false;
}

bool Board::isCheckmate(bool whiteKing) {
    Piece* king = getKing(whiteKing);
    if (!king) return false; // Nếu không tìm thấy quân vua, không phải checkmate

    // Kiểm tra nếu quân vua có thể di chuyển đến ô khác không bị chiếu
    if (kingCanMove(king, pieces)) return false;  // Nếu vua có thể di chuyển ra khỏi chiếu

    // Nếu vua không thể di chuyển ra khỏi chiếu, kiểm tra xem có quân nào có thể chặn chiếu không
    for (Piece* p : pieces) {
        if (p->isWhite == whiteKing) {  // Kiểm tra quân đối phương
            // Kiểm tra xem quân này có thể di chuyển đến vị trí cản chiếu không
            int kingRow = king->getRow();
            int kingCol = king->getCol();
            if (p->canMoveTo(kingRow, kingCol, pieces)) {
                return false;  // Nếu có quân đối phương có thể cản chiếu
            }
        }
    }

    // Nếu không có quân nào có thể cản chiếu và vua không thể di chuyển, đó là checkmate
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



