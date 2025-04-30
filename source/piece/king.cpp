#include "king.h"
#include "rook.h"
#include <cmath>
#include <algorithm>

King::King(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, KING) {}

bool King::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int dx = std::abs(newRow - row);
    int dy = std::abs(newCol - col);
     // King move check
    if (dx <= 1 && dy <= 1 && (dx + dy > 0)) {
        for (Piece* p : pieces) {
            if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite) {
                return false;
            }
        }
        return true;
    }

    // castling
    if (hasMoved && newRow == row) {
        std::vector<Piece*> enemyPieces;
        for (Piece* p : pieces) {
            if (p->isWhite != isWhite) {
                enemyPieces.push_back(p);
            }
        }
        auto isSquareUnderAttack = [&](int r, int c) -> bool {
            for (Piece* enemy : enemyPieces) {
                if (enemy->isValidMove(r, c, pieces)) {
                    return true;
                }
            }
            return false;
        };

        // ==== Kingside Castling ====
        if (newCol == col + 2) {
            // Check if the path between the king and the rook is clear
            bool pathClear = true;
            for (Piece* p : pieces) {
                if (p->getRow() == row && (p->getCol() == col + 1 || p->getCol() == col + 2)) {
                    pathClear = false;
                    break;
                }
            }
            if (!pathClear) {
                return false;
            }
            if (isSquareUnderAttack(row, col) || isSquareUnderAttack(row, col + 1) || isSquareUnderAttack(row, col + 2)) {
                return false;
            }

            // Check if the rook is in the correct position and hasn't moved
            for (Piece* p : pieces) {
                if (p->getType() == ROOK && p->isWhite == isWhite && p->getRow() == row && p->getCol() == col + 3) {
                    Rook* rook = dynamic_cast<Rook*>(p);
                    if (rook && rook->hasMoved) {
                        return true;
                    }
                }
            }

        }

        // ==== Queenside Castling ====
        if (newCol == col - 2) {
            bool pathClear = true;
            for (Piece* p : pieces) {
                if (p->getRow() == row &&
                    (p->getCol() == col - 1 || p->getCol() == col - 2 || p->getCol() == col - 3)) {

                    pathClear = false;
                    break;
                }
            }
            if (!pathClear) {
                return false;
            }
            if (isSquareUnderAttack(row, col) || isSquareUnderAttack(row, col - 1) || isSquareUnderAttack(row, col - 2)) {

                return false;
            }
            for (Piece* p : pieces) {
                if (p->getType() == ROOK && p->isWhite == isWhite && p->getRow() == row && p->getCol() == col - 4) {
                    Rook* rook = dynamic_cast<Rook*>(p);
                    if (rook && rook->hasMoved) {
                        return true;
                    }
                }
            }

        }
    }


    return false;
}


