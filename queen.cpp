#include "queen.h"
bool Queen::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    if (newRow == row || newCol == col) {
        return Rook::isValidMove(newRow, newCol, pieces);  // Di chuyển theo đường thẳng (ngang hoặc dọc)
    }
    if (abs(newRow - row) == abs(newCol - col)) {
        return Bishop::isValidMove(newRow, newCol, pieces);  // Di chuyển theo đường chéo
    }
    return false;
}
