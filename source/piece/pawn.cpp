#include "pawn.h"
#include <iostream>
#include <cmath>
Pawn::Pawn(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, PAWN) {}
bool Pawn::canMoveTo(int newRow, int newCol,const std::vector<Piece*>& pieces) {
    int direction = isWhite ? 1 : -1;
    // 1. Đi thẳng 1 ô
    if (newCol == col && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol) {
                return false;  // Có vật cản phía trước
            }
        }
        return true;
    }

    // 2. Đi 2 ô nếu ở vị trí bắt đầu
    if (newCol == col && newRow == row + 2 * direction &&
        ((isWhite && row == 1) || (!isWhite && row == 6))) {
        for (Piece* piece : pieces) {
            if ((piece->getRow() == row + direction && piece->getCol() == col) ||
                (piece->getRow() == newRow && piece->getCol() == newCol)) {
                return false;  // Có vật cản
            }
        }
        return true;
    }

    // 3. Ăn chéo
    if (std::abs(newCol - col) == 1 && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol &&
                piece->isWhite != isWhite) {
                return true;  // Ăn quân đối phương
            }
        }
    }

    return false;  // Mọi trường hợp khác đều sai luật
}

bool Pawn::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int direction = isWhite ? 1 : -1;
    // 1. Đi thẳng 1 ô
    if (newCol == col && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol) {
                return false;  // Có vật cản phía trước
            }
        }
        return true;
    }

    // 2. Đi 2 ô nếu ở vị trí bắt đầu
    if (newCol == col && newRow == row + 2 * direction &&
        ((isWhite && row == 1) || (!isWhite && row == 6))) {
        for (Piece* piece : pieces) {
            if ((piece->getRow() == row + direction && piece->getCol() == col) ||
                (piece->getRow() == newRow && piece->getCol() == newCol)) {
                return false;  // Có vật cản
            }
        }
        return true;
    }

    // 3. Ăn chéo
    if (std::abs(newCol - col) == 1 && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol &&
                piece->isWhite != isWhite) {
                return true;  // Ăn quân đối phương
            }
        }
    }

    return false;  // Mọi trường hợp khác đều sai luật
}
