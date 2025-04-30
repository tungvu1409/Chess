#include "bishop.h"
#include <cmath>
Bishop::Bishop(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, BISHOP) {}

bool Bishop::canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) {
    if (newRow == row && newCol == col)
        return false;

    if (newRow < 0 || newRow > 7 || newCol < 0 || newCol > 7)
        return false;
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);
    if (dx != dy)
        return false;
    int stepRow = (newRow > row) ? 1 : -1;
    int stepCol = (newCol > col) ? 1 : -1;

    int r = row + stepRow;
    int c = col + stepCol;
    while (r != newRow && c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c)
                return false;
        }
        r += stepRow;
        c += stepCol;
    }

    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol) {
            return p->isWhite != isWhite;
        }
    }
    return true;
}




// Check whether Bishop piece is valid or not
bool Bishop::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {

    if (newRow == row && newCol == col)
        return false;

    if (newRow < 0 || newRow > 7 || newCol < 0 || newCol > 7)
        return false;

    // check if the bishop's move is along a diagonal
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);
    if (dx != dy)
        return false;
    // Determine the direction to step in rows and columns (up/down, left/right)
    int stepRow = (newRow > row) ? 1 : -1;
    int stepCol = (newCol > col) ? 1 : -1;

    // Checks for obstacles along the diagonal path to the target square
    int r = row + stepRow;
    int c = col + stepCol;
    while (r != newRow && c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c)
                return false;
        }
        r += stepRow;
        c += stepCol;
    }

    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol) {
            return p->isWhite != isWhite;            // can capture i it's the opponent's piece
        }
    }
    return true;
}


