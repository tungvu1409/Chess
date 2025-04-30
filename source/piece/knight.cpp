#include "knight.h"
#include <cmath>
Knight::Knight(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, KNIGHT) {}

bool Knight::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {

    if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
        return false;
    }
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);

    // A Knight moves in an "L" shape: (2, 1) or (1, 2)
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol) {
                if (piece->isWhite == isWhite) {
                    return false; // If the piece is of the same color, cannot move to that square
                }

                return true;
            }
        }
        return true;
    }
    return false;
}

bool Knight::canMoveTo(int newRow, int newCol,const std::vector<Piece*>& pieces) {

     if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
        return false;
    }
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);

    // A Knight moves in an "L" shape: (2, 1) or (1, 2)
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol) {
                if (piece->isWhite == isWhite) {
                    return false; // If the piece is of the same color, cannot move to that square
                }

                return true;
            }
        }
        return true;
    }
    return false;
}
