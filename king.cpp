#include "king.h"
bool King::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int rowDiff = abs(newRow - row);
    int colDiff = abs(newCol - col);

    // Vua chỉ có thể di chuyển 1 ô trong tất cả các hướng
    if ((rowDiff == 1 && colDiff <= 1) || (colDiff == 1 && rowDiff <= 1)) {
        // Kiểm tra xem ô đích có quân cờ nào không
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol && piece->isWhite == isWhite) {
                return false;  // Không thể di chuyển nếu ô có quân cùng màu
            }
        }
        return true;
    }
    return false;
}
