#include "bishop.h"
bool Bishop::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    if (abs(newRow - row) == abs(newCol - col)) {  // Di chuyển theo đường chéo
        int rowStep = (newRow > row) ? 1 : -1;
        int colStep = (newCol > col) ? 1 : -1;

        for (int i = 1; i < abs(newRow - row); ++i) {
            if (getPieceAt(row + i * rowStep, col + i * colStep, pieces) != nullptr) {
                return false;  // Kiểm tra đường đi có bị cản trở không
            }
        }
        return true;
    }
    return false;
}
