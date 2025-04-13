#include "king.h"
#include <cmath>
#include <algorithm>

King::King(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, KING) {}

bool King::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int dx = abs(newRow - row);
    int dy = abs(newCol - col);

    // Vua chỉ được đi 1 ô theo mọi hướng
    if (dx <= 1 && dy <= 1 && (dx + dy > 0)) {
        // Không được ăn quân cùng màu
        for (Piece* p : pieces) {
            if (p->getRow() == newRow && p->getCol() == newCol && p->isWhite == isWhite)
                return false;
        }

        // Tạm thời thử di chuyển vua để kiểm tra có bị chiếu không
        int oldRow = row, oldCol = col;
        Piece* captured = nullptr;

        // Kiểm tra xem có quân nào ở đích để ăn không
        for (auto it = pieces.begin(); it != pieces.end(); ++it) {
            if ((*it)->getRow() == newRow && (*it)->getCol() == newCol) {
                captured = *it;
                pieces.erase(it);
                break;
            }
        }

        row = newRow;
        col = newCol;

        // Kiểm tra có bị chiếu sau khi di chuyển không
        bool inCheck = false;
        for (Piece* p : pieces) {
            if (p->isWhite != isWhite && p->canMoveTo(newRow, newCol, pieces)) {
                inCheck = true;
                break;
            }
        }

        // Quay lại vị trí cũ và khôi phục quân bị ăn (nếu có)
        row = oldRow;
        col = oldCol;
        if (captured) {
            pieces.push_back(captured);
        }

        return !inCheck;
    }

    return false;
}
