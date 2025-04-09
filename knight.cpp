#include "knight.h"

bool Knight::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int rowDiff = abs(newRow - row);
    int colDiff = abs(newCol - col);

    // Mã di chuyển theo hình chữ L (2 ô theo một hướng và 1 ô theo hướng còn lại)
    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol && piece->isWhite == isWhite) {
                return false;  // Nếu ô đích có quân cùng màu, không thể di chuyển
            }
        }
        return true;
    }
    return false;
}
