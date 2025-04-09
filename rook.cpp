#include "rook.h"

bool Rook::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    if (newRow == row) {  // Di chuyển ngang
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() > std::min(col, newCol) && piece->getCol() < std::max(col, newCol)) {
                return false;  // Kiểm tra đường đi có bị cản trở không
            }
        }
        return true;
    }
    if (newCol == col) {  // Di chuyển dọc
        for (Piece* piece : pieces) {
            if (piece->getCol() == newCol && piece->getRow() > std::min(row, newRow) && piece->getRow() < std::max(row, newRow)) {
                return false;  // Kiểm tra đường đi có bị cản trở không
            }
        }
        return true;
    }
    return false;  // Không di chuyển hợp lệ
}
