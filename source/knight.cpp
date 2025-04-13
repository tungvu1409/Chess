#include "knight.h"
#include <cmath>
Knight::Knight(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, KNIGHT) {}

bool Knight::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol && piece->isWhite == isWhite)
                return false;
        }
        return true;
    }
    return false;
}
bool Knight::canMoveTo(int newRow, int newCol,const std::vector<Piece*>& pieces) {
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol && piece->isWhite == isWhite)
                return false;
        }
        return true;
    }
    return false;
}
