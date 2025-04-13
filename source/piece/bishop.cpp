#include "bishop.h"
#include <cmath>
Bishop::Bishop(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, BISHOP) {}
bool Bishop::canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) {
    // Kiểm tra nếu di chuyển chéo hợp lệ (chênh lệch cột và hàng phải bằng nhau)
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);
    if (dx != dy) return false;

    // Xác định bước đi theo chiều hàng và cột
    int stepRow = (newRow > row) ? 1 : -1;
    int stepCol = (newCol > col) ? 1 : -1;

    // Kiểm tra các ô trên đường chéo
    int r = row + stepRow;
    int c = col + stepCol;

    // Duyệt qua các ô trên đường chéo từ ô hiện tại đến ô đích
    int steps = abs(newRow - row);  // Số bước cần di chuyển
    for (int i = 0; i < steps; ++i) {
        // Kiểm tra nếu có quân nào chắn đường
        for (Piece* p : pieces) {
            if (p->getRow() == r && p->getCol() == c)
                return false; // Có quân chắn đường, không thể di chuyển
        }

        // Di chuyển tới ô tiếp theo trên đường chéo
        r += stepRow;
        c += stepCol;
    }

    // Kiểm tra ô đích có quân cờ cùng màu không (không thể ăn quân cùng màu)
    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite)
            return false; // Có quân cờ cùng màu ở ô đích, không thể di chuyển
    }

    return true;  // Nếu không có vấn đề, trả về true
}



bool Bishop::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    // Kiểm tra nếu di chuyển chéo hợp lệ (chênh lệch cột và hàng phải bằng nhau)
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);
    if (dx != dy) return false;

    // Xác định bước đi theo chiều hàng và cột
    int stepRow = (newRow > row) ? 1 : -1;
    int stepCol = (newCol > col) ? 1 : -1;

    // Kiểm tra các ô trên đường chéo
    int r = row + stepRow;
    int c = col + stepCol;

    // Duyệt qua các ô trên đường chéo từ ô hiện tại đến ô đích
    int steps = abs(newRow - row);  // Số bước cần di chuyển
    for (int i = 0; i < steps; ++i) {

        // Kiểm tra nếu có quân nào chắn đường
        for (Piece* p : pieces) {

            if (p->getRow() == r && p->getCol() == c)
                return false; // Có quân chắn đường, không thể di chuyển
        }

        // Di chuyển tới ô tiếp theo trên đường chéo
        r += stepRow;
        c += stepCol;
    }

    // Kiểm tra ô đích có quân cờ cùng màu không (không thể ăn quân cùng màu)
    for (Piece* p : pieces) {
        if (p->getRow() == newRow && p->getCol() == newCol) {
            // Nếu có quân ở ô đích và quân đó cùng màu thì không thể di chuyển (ăn quân đồng màu)
            if (p->isWhite == isWhite) {
                return false;  // Không thể ăn quân cùng màu
            } else {
                // Nếu quân đó khác màu thì có thể ăn quân đó
                return true;
            }
        }
    }

    return true;  // Nếu không có vấn đề, trả về true
}

