
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include <cmath>

Queen::Queen(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, QUEEN) {}

bool Queen::canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) {
    if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
        return false;
    }

    int dRow = newRow - row;
    int dCol = newCol - col;

    if (dRow != 0 && dCol != 0 && abs(dRow) != abs(dCol)) return false;

    int stepRow = (dRow == 0) ? 0 : (dRow > 0 ? 1 : -1);
    int stepCol = (dCol == 0) ? 0 : (dCol > 0 ? 1 : -1);
    int r = row + stepRow;
    int c = col + stepCol;

    while (r != newRow || c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c) return false;
        }
        r += stepRow;
        c += stepCol;
    }

    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite) {
            return false;
        }
    }

    return true;
}

bool Queen::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
        return false;
    }

    int dRow = newRow - row;
    int dCol = newCol - col;

    if (dRow != 0 && dCol != 0 && abs(dRow) != abs(dCol)) return false;

    int stepRow = (dRow == 0) ? 0 : (dRow > 0 ? 1 : -1);
    int stepCol = (dCol == 0) ? 0 : (dCol > 0 ? 1 : -1);
    int r = row + stepRow;
    int c = col + stepCol;

    while (r != newRow || c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c) return false;
        }
        r += stepRow;
        c += stepCol;
    }

    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite) {
            return false;
        }
    }

    return true;
}

