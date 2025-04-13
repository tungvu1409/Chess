#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include <cmath>
static Piece* findPieceAt(int r, int c, const std::vector<Piece*>& pieces) {
    for (Piece* p : pieces)
        if (p->getRow() == r && p->getCol() == c) return p;
    return nullptr;
}
Queen::Queen(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, QUEEN) {}
bool Queen::canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) {
    // Nếu di chuyển như Rook (hàng hoặc cột)
    if (newRow == row || newCol == col) {
        int dr = (newRow > row) ? 1 : (newRow < row ? -1 : 0);
        int dc = (newCol > col) ? 1 : (newCol < col ? -1 : 0);
        int r = row + dr, c = col + dc;
        while (r != newRow || c != newCol) {
            if (findPieceAt(r, c, pieces)) return false;
            r += dr; c += dc;
        }
        return true;
    }

    // Nếu di chuyển như Bishop (đường chéo)
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);
    if (dx != dy) return false;  // Đảm bảo là di chuyển đường chéo

    int stepRow = (newRow > row) ? 1 : -1;
    int stepCol = (newCol > col) ? 1 : -1;

    int r = row + stepRow;
    int c = col + stepCol;

    // Kiểm tra các ô trên đường chéo trước khi đến ô đích
    while (r != newRow && c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c) {
                return false;  // Có quân cờ khác trên đường đi
            }
        }
        r += stepRow;
        c += stepCol;
    }

    // Kiểm tra ô đích, nếu có quân cờ cùng màu ở đó, không thể di chuyển
    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite) {
            return false;  // Ô đích có quân cờ cùng màu
        }
    }

    return true;
}

bool Queen::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int dRow = newRow - row;
    int dCol = newCol - col;

    if (dRow != 0 && dCol != 0 && abs(dRow) != abs(dCol)) return false;

    int stepRow = (dRow == 0) ? 0 : (dRow > 0 ? 1 : -1);
    int stepCol = (dCol == 0) ? 0 : (dCol > 0 ? 1 : -1);
    int r = row + stepRow;
    int c = col + stepCol;

    while (r != newRow || c != newCol) {
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c) return false;  // Bị cản đường
        }
        r += stepRow;
        c += stepCol;
    }

    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite) {
            return false; // Không ăn quân cùng màu
        }
    }

    return true;
}

