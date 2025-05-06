#include "board.h"
#include <iostream>
#include <algorithm>

Piece* Board::getPieceAt(int row, int col) {
    for (Piece* piece : pieces) {
        if (piece->getRow() == row && piece->getCol() == col) {
            return piece;
        }
    }
    return nullptr;
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cerr << "Unable to load image! Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

// load piece textures and set up pieces on the board
void Board::init(SDL_Renderer* renderer) {
    pieceTextures[0] = loadTexture("/ChessGame/img/king1.png", renderer);
    pieceTextures[1] = loadTexture("/ChessGame/img/queen1.png", renderer);
    pieceTextures[2] = loadTexture("/ChessGame/img/rook1.png", renderer);
    pieceTextures[3] = loadTexture("/ChessGame/img/bishop1.png", renderer);
    pieceTextures[4] = loadTexture("/ChessGame/img/knight1.png", renderer);
    pieceTextures[5] = loadTexture("/ChessGame/img/pawn1.png", renderer);

    pieceTextures[6] = loadTexture("/ChessGame/img/king2.png", renderer);
    pieceTextures[7] = loadTexture("/ChessGame/img/queen2.png", renderer);
    pieceTextures[8] = loadTexture("/ChessGame/img/rook2.png", renderer);
    pieceTextures[9] = loadTexture("/ChessGame/img/bishop2.png", renderer);
    pieceTextures[10] = loadTexture("/ChessGame/img/knight2.png", renderer);
    pieceTextures[11] = loadTexture("/ChessGame/img/pawn2.png", renderer);

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

// =============================================== DRAW BOARD ======================================================
void Board::draw(SDL_Renderer* renderer) {
    int tileSize = 75;
    // draw board
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 107, 142, 35, 255); // olive color
            } else {
                SDL_SetRenderDrawColor(renderer, 227, 218, 201, 255); // bone color
            }

            SDL_Rect rect = { col * tileSize, row * tileSize, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &rect); // create a filled rectangle on the screen with the current color
        }
    }


    // draw valid move suggestions
    for (const auto& move : validMoves) {
        int row = move.first;
        int col = move.second;
        Piece* target = getPieceAt(row, col);
        // If there is a opponent's piece at the target position => highlight
        if (target && target->isWhite != turnIsWhite) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
            SDL_Rect attackRect = {col * tileSize, row * tileSize, tileSize, tileSize};
            SDL_RenderFillRect(renderer, &attackRect);
        }
        // draw a white dot
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
            SDL_Rect dot = {col * tileSize + tileSize / 2 - 6, row * tileSize + tileSize / 2 - 6, 12, 12};
            SDL_RenderFillRect(renderer, &dot);
        }
    }

    // Highlight king in check
    Piece* king = getKing(turnIsWhite);
    if (king && isInCheck(turnIsWhite)) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
        SDL_Rect kingRect = {king->getCol() * tileSize, king->getRow() * tileSize, tileSize, tileSize};
        SDL_RenderFillRect(renderer, &kingRect);
    }

    // draw pieces
    for (Piece* piece : pieces) {
        SDL_Rect pieceRect = {
            piece->getCol() * tileSize,
            piece->getRow() * tileSize,
            tileSize,
            tileSize
        };

        SDL_RenderCopy(renderer, pieceTextures[piece->getType() + (piece->isWhite ? 0 : 6)], NULL, &pieceRect);
    }

}

void Board::handleMouseClick(int x, int y, SDL_Renderer* renderer) {
    if (gameOver) return;
    int row = y / 75;
    int col = x / 75;

    if (!selectedPiece) {
        bool foundPiece = false;
        for (Piece* piece : pieces) {
            if (piece->getRow() == row && piece->getCol() == col && piece->isWhite == turnIsWhite) {
                selectedPiece = piece;
                foundPiece = true;
                oldRow = piece->getRow();
                oldCol = piece->getCol();


                validMoves.clear();
                for (int r = 0; r < 8; ++r) {
                    for (int c = 0; c < 8; ++c) {
                        if (piece->isValidMove(r, c, pieces)) {
                            validMoves.push_back({r, c});
                        }
                    }
                }
                break;
            }
        }

        if (!foundPiece) {
            std::cout << "Please choose a valid piece" << std::endl;
        }
    } else {

        bool isValid = false;
        for (const auto& move : validMoves) {
            if (move.first == row && move.second == col) {
                isValid = true;
                break;
            }
        }

        if (isValid) {

            // ============================== HANDLING CASTLE =====================================
            if (selectedPiece->getType() == KING && abs(col - oldCol) == 2) {
                // Kingside castling
                if (col > oldCol) {
                    selectedPiece->move(row, 6);
                    for (Piece* p : pieces) {
                        if (p->getType() == ROOK && p->getRow() == row && p->getCol() == 7 && !p->hasMoved) {
                            p->move(row, 5);
                            break;
                        }
                    }
                }
                // Queenside castling
                else {
                    selectedPiece->move(row, 2);
                    for (Piece* p : pieces) {
                        if (p->getType() == ROOK && p->getRow() == row && p->getCol() == 0 && !p->hasMoved) {
                            p->move(row, 3);
                            break;
                        }
                    }
                }
            }

            // ================================= PROCESSING EN PASSANT MOVE ==============================
            Pawn* pawn = dynamic_cast<Pawn*>(selectedPiece);
            if (pawn != nullptr) {
                if (std::abs(col - pawn->getCol()) == 1 && row - pawn->getRow() == (pawn->isWhite ? 1 : -1)) {
                    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
                        Piece* piece = *it;
                        if (piece->getRow() == pawn->getRow() && piece->getCol() == col &&
                            piece->isWhite != pawn->isWhite && piece->twoStepped) {
                            std::cout << "En Passant";
                            delete piece;
                            pieces.erase(it);
                            break;
                        }
                    }
                }
            }

            // ============================== CAPTURE OPPONENT'S PIECE ================================
            for (auto it = pieces.begin(); it != pieces.end(); ++it) {
                Piece* piece = *it;
                if (piece->getRow() == row && piece->getCol() == col && piece->isWhite != selectedPiece->isWhite) {
                    std::cout << "Captured opponent's piece at (" << row << ", " << col << ")\n";
                    if (piece->getType() == KING) {
                        std::cout << (turnIsWhite ? "White wins!" : "Black wins!") << std::endl;
                        gameOver = true;
                    }
                    delete piece;
                    pieces.erase(it);
                    break;
                }
            }

            // =================================== EXECUTE THE MOVE =========================================
            int oldRow = selectedPiece->getRow();
            int oldCol = selectedPiece->getCol();
            selectedPiece->move(row, col);
            gfx.playMoveSound();   // sound effect for the move

            // ============================== CHECK FOR CHECK AFTER THE MOVE ===============================
            bool currentlyInCheck = isInCheck(turnIsWhite);
            if (currentlyInCheck) {
                std::cout << "Check! The king is in check!" << std::endl;
                selectedPiece->move(oldRow, oldCol);
                return;
            }
            // ================================ CHECK FOR DRAW CONDITIONS ===================================
            if (isStalemate() || isInsufficientMaterial()) {
                std::cout << "The game has ended in a draw!\n";
                gameOver = true;
            }
            // =============================== CHECK FOR CHECKMATE CONDITON ==================================
            if (isCheckmate(!turnIsWhite)) {
                std::cout << "Checkmate! ";
                std::cout << (turnIsWhite ? "White wins!" : "Black wins!") << std::endl;
                gameOver = true;
            }

            // ================================== CHECK FOR PROMOTION  ========================================
            if (selectedPiece->getType() == PAWN && ((selectedPiece->isWhite && row == 7) || (!selectedPiece->isWhite && row == 0))) {
                std::cout << "Pawn reached the last rank." << std::endl;
                Piece* newPiece = showPromotionMenu(row, col, selectedPiece->isWhite, renderer);
                if (newPiece) {
                    pieces.push_back(newPiece);
                    auto it = std::find(pieces.begin(), pieces.end(), selectedPiece);
                    if (it != pieces.end()) {
                        pieces.erase(it);
                    }
                    delete selectedPiece;
                    selectedPiece = nullptr;
                    std::cout << "Pawn promoted successfully." << std::endl;
                }
            }

            // ==================================== SWITCH TURN ==========================================
            turnIsWhite = !turnIsWhite;
            selectedPiece = nullptr;
            validMoves.clear();
        } else {
            std::cout << "Invalid move!" << std::endl;

            selectedPiece = nullptr;
            validMoves.clear();
        }
    }
}



bool Board::isInCheck(bool whiteKing) {
    if (gameOver) return false;

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


bool Board::kingCanMove(Piece* king, std::vector<Piece*>& pieces) {
    int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1},
                            {0, -1},           {0, 1},
                            {1, -1},  {1, 0},  {1, 1}};

    int oldRow = king->getRow();
    int oldCol = king->getCol();

    for (auto& dir : directions) {
        int newRow = oldRow + dir[0];
        int newCol = oldCol + dir[1];


        if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
            continue;
        }


        Piece* target = getPieceAt(newRow, newCol);
        if (target && target->isWhite == king->isWhite) {
            continue;
        }

        bool isValidMove = true;
        std::vector<Piece*> tempPieces = pieces;

        if (target) {

            tempPieces.erase(std::remove(tempPieces.begin(), tempPieces.end(), target), tempPieces.end());
        }


        king->move(newRow, newCol);


        if (isInCheck(king->isWhite)) {
            isValidMove = false;
        }


        king->move(oldRow, oldCol);

        if (isValidMove) {
            return true;
        }
    }

    return false;
}
bool Board::isSquareUnderAttack(int row, int col, bool whiteKing) {
    for (Piece* p : pieces) {
        if (p->isWhite != whiteKing && p->canMoveTo(row, col, pieces)) {
            return true;
        }
    }
    return false;
}

// ======================================== CHECKMATE FUNCTION ===============================================
bool Board::isCheckmate(bool whiteKing) {

    if (!isInCheck(whiteKing)) {
        return false;
    }


    Piece* king = getKing(whiteKing);
    if (!king) return false;

    if (kingCanMove(king, pieces)) {
        return false;
    }


    for (Piece* p : pieces) {
        if (p->isWhite == whiteKing && p->getType() != KING) {
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    if (p->canMoveTo(row, col, pieces)) {

                        Piece* captured = getPieceAt(row, col);
                        int oldRow = p->getRow();
                        int oldCol = p->getCol();


                        if (captured) {
                            pieces.erase(std::remove(pieces.begin(), pieces.end(), captured), pieces.end());
                        }
                        p->move(row, col);

                        bool stillInCheck = isInCheck(whiteKing);


                        p->move(oldRow, oldCol);
                        if (captured) {
                            pieces.push_back(captured);
                        }

                        if (!stillInCheck) {
                            return false;
                        }
                    }
                }
            }
        }
    }


    return true;
}


bool Board::isKingInCheck(bool isWhite)
{

    King* king = nullptr;
    for (Piece* piece : pieces) {
        if (piece->getType() == KING && piece->isWhite == isWhite) {
            king = dynamic_cast<King*>(piece);
            break;
        }
    }
    if (!king) {
        return false;
    }

    for (Piece* piece : pieces) {
        if (piece->isWhite != isWhite) {
            if (piece->isValidMove(king->getRow(), king->getCol(), pieces)) {
                return true;
            }
        }
    }
    return false;
}
// ===========================================  PROMOTION =====================================================
Piece* Board::showPromotionMenu(int row, int col, bool turnIsWhite, SDL_Renderer* renderer) {
    std::cout << "Inside promotion menu" << std::endl;

    bool promotionChosen = false;
    SDL_Event event;

    // Define button areas for promotion choices
    SDL_Rect queenButton = {100, 400, 100, 50};
    SDL_Rect rookButton = {210, 400, 100, 50};
    SDL_Rect knightButton = {320, 400, 100, 50};
    SDL_Rect bishopButton = {430, 400, 100, 50};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &queenButton);
    SDL_RenderFillRect(renderer, &rookButton);
    SDL_RenderFillRect(renderer, &knightButton);
    SDL_RenderFillRect(renderer, &bishopButton);


    SDL_Rect textureRect = {100, 400, 50, 50};

    SDL_RenderCopy(renderer, pieceTextures[1], nullptr, &textureRect);  // Draw Queen
    textureRect.x = 210;
    SDL_RenderCopy(renderer, pieceTextures[2], nullptr, &textureRect);  // Draw Rook
    textureRect.x = 320;
    SDL_RenderCopy(renderer, pieceTextures[4], nullptr, &textureRect);  // Draw Knight
    textureRect.x = 430;
    SDL_RenderCopy(renderer, pieceTextures[3], nullptr, &textureRect);  // Draw Bishop

    SDL_RenderPresent(renderer);

    // Loop until the player clicks one of the promotion buttons
    while (!promotionChosen) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "User closed the promotion menu" << std::endl;
                return nullptr;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Kiểm tra các nút thăng cấp
                if (x >= queenButton.x && x <= queenButton.x + queenButton.w &&
                    y >= queenButton.y && y <= queenButton.y + queenButton.h) {
                    std::cout << "Queen selected!" << std::endl;
                    promotionChosen = true;
                    return new Queen(row, col, turnIsWhite);
                }
                if (x >= rookButton.x && x <= rookButton.x + rookButton.w &&
                    y >= rookButton.y && y <= rookButton.y + rookButton.h) {
                    std::cout << "Rook selected!" << std::endl;
                    promotionChosen = true;
                    return new Rook(row, col, turnIsWhite);
                }
                if (x >= knightButton.x && x <= knightButton.x + knightButton.w &&
                    y >= knightButton.y && y <= knightButton.y + knightButton.h) {
                    std::cout << "Knight selected!" << std::endl;
                    promotionChosen = true;
                    return new Knight(row, col, turnIsWhite);
                }
                if (x >= bishopButton.x && x <= bishopButton.x + bishopButton.w &&
                    y >= bishopButton.y && y <= bishopButton.y + bishopButton.h) {
                    std::cout << "Bishop selected!" << std::endl;
                    promotionChosen = true;
                    return new Bishop(row, col, turnIsWhite);
                }
            }
        }
    }

    return nullptr;
}

// ========================================== CHECK DRAW =================================================

bool Board::isStalemate()
{
    if (isKingInCheck(turnIsWhite)) {
        return false;
    }

    // Check if there are any valid moves for the current player's pieces
    for (Piece* piece : pieces) {
        if (piece->isWhite == turnIsWhite) {
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    if (piece->isValidMove(row, col, pieces)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::isInsufficientMaterial()
{
    int whiteCount = 0, blackCount = 0;
    bool whiteHasMinor = false, blackHasMinor = false;

    for (Piece* p : pieces) {
        if (p->getType() != KING) {
            if (p->isWhite) {
                whiteCount++;
                if (p->getType() == BISHOP || p->getType() == KNIGHT)
                    whiteHasMinor = true;
            } else {
                blackCount++;
                if (p->getType() == BISHOP || p->getType() == KNIGHT)
                    blackHasMinor = true;
            }
        }
    }

    // both players only have their kings left => draw
    if (whiteCount == 0 && blackCount == 0) return true;

    // only a king and a bishop or a knight), and the opponent has no pieces left => draw
    if ((whiteCount == 1 && whiteHasMinor && blackCount == 0) ||
        (blackCount == 1 && blackHasMinor && whiteCount == 0))
        return true;

    return false;
}

// ========================================== REMATCH GAME ==================================================
void Board::reset(int timeLimit) {
    // Clear out the old pieces
    for (auto piece : pieces) {
        delete piece;
    }
    pieces.clear();

    // Reset each player’s clock
    whiteTimeLeft = timeLimit;
    blackTimeLeft = timeLimit;

    // white pieces
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

    // black pieces
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

    // Reset turn and game state
    turnIsWhite = true;
    gameOver = false;
}

