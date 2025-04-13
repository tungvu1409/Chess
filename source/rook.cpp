#include "rook.h"

Rook::Rook(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, ROOK) {}

bool Rook::canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) {
    // Rook chỉ đi theo hàng hoặc cột
    if (newRow != row && newCol != col) return false;

    int stepRow = (newRow > row) ? 1 : (newRow < row ? -1 : 0);
    int stepCol = (newCol > col) ? 1 : (newCol < col ? -1 : 0);

    int r = row + stepRow;
    int c = col + stepCol;
    while (r != newRow || c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c) {
                return false; // bị chặn
            }
        }
        r += stepRow;
        c += stepCol;
    }

    // Kiểm tra nếu ô đích có đồng đội thì không thể đi
    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite) {
            return false;
        }
    }

    return true;
}

bool Rook::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    if (row != newRow && col != newCol)
        return false;

    int stepRow = (newRow - row) == 0 ? 0 : ((newRow - row > 0) ? 1 : -1);
    int stepCol = (newCol - col) == 0 ? 0 : ((newCol - col > 0) ? 1 : -1);

    int r = row + stepRow;
    int c = col + stepCol;

    while (r != newRow || c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c)
                return false;
        }
        r += stepRow;
        c += stepCol;
    }

    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite)
            return false;
    }

    return true;
}
