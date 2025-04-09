#include "pawn.h"

bool Pawn::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int direction = isWhite ? -1 : 1; // Quân Tốt di chuyển lên cho quân trắng, xuống cho quân đen

    // Kiểm tra di chuyển 1 ô thẳng phía trước
    if (newRow == row + direction && newCol == col) {
        // Kiểm tra ô phía trước có trống không (không có quân cờ cùng màu)
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol) {
                if (piece->isWhite == isWhite) {
                    std::cout << "Ô này có quân cờ cùng màu.\n";
                    return false; // Không thể di chuyển nếu có quân cờ cùng màu ở ô đích
                }
                return true; // Di chuyển hợp lệ nếu ô đích trống hoặc có quân cờ đối phương
            }
        }
        return true; // Nếu không có quân cờ nào ở ô đích, di chuyển hợp lệ
    }

    // Kiểm tra di chuyển 2 ô lần đầu (nếu quân Tốt chưa di chuyển)
    if (newCol == col && newRow == row + 2 * direction && (row == 1 || row == 6)) {
        // Kiểm tra ô phía trước có trống không và ô thứ hai cũng phải trống
        for (Piece* piece : pieces) {
            if ((piece->getRow() == newRow && piece->getCol() == newCol) ||
                (piece->getRow() == row + direction && piece->getCol() == col)) {
                std::cout << "Có vật cản, không thể di chuyển.\n";
                return false; // Có vật cản, không di chuyển được
            }
        }
        return true; // Di chuyển hợp lệ nếu cả hai ô đều trống
    }

    // Kiểm tra ăn quân đối phương (di chuyển chéo)
    if (abs(newCol - col) == 1 && newRow == row + direction) {
        // Kiểm tra ô đích có quân đối phương không
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol && piece->isWhite != isWhite) {
                return true; // Có quân đối phương để ăn
            }
        }
        std::cout << "Không có quân đối phương để ăn.\n";
        return false; // Không có quân đối phương để ăn
    }

    std::cout << "Nước đi không hợp lệ.\n";
    return false; // Không di chuyển hợp lệ
}
