#include "pawn.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

Pawn::Pawn(int row, int col, bool isWhite)
    : Piece(row, col, isWhite, PAWN) {}

bool Pawn::canMoveTo(int newRow, int newCol,const std::vector<Piece*>& pieces) {
    int direction = isWhite ? 1 : -1;
    // 1. Đi thẳng 1 ô
    if (newCol == col && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol) {
                return false;
            }
        }
        return true;
    }

    if (newCol == col && newRow == row + 2 * direction &&
        ((isWhite && row == 1) || (!isWhite && row == 6))) {
        for (Piece* piece : pieces) {
            if ((piece->getRow() == row + direction && piece->getCol() == col) ||
                (piece->getRow() == newRow && piece->getCol() == newCol)) {
                return false;
            }
        }
        return true;
    }


    if (std::abs(newCol - col) == 1 && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol &&
                piece->isWhite != isWhite) {
                return true;
            }
        }
    }

    return false;
}
void Pawn::move(int newRow, int newCol) {

    if (std::abs(newRow - row) == 2) {
        twoStepped = true;
    } else {
        twoStepped = false;
    }


    row = newRow;
    col = newCol;
}
bool Pawn::isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
    int direction = isWhite ? 1 : -1;

    // Check if the pawn moves forward by 1 square (straight)
    if (newCol == col && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol) {
                return false;
            }
        }
        return true;
    }

    // Check if the pawn moves forward by 2 squares from its starting position
    if (newCol == col && newRow == row + 2 * direction &&
        ((isWhite && row == 1) || (!isWhite && row == 6))) {
        for (Piece* piece : pieces) {
            if ((piece->getRow() == row + direction && piece->getCol() == col) ||
                (piece->getRow() == newRow && piece->getCol() == newCol)) {
                return false;
            }
        }
        return true;
    }

    // Check if the pawn moves diagonally to capture an opponent's piece
    if (std::abs(newCol - col) == 1 && newRow == row + direction) {
        for (Piece* piece : pieces) {
            if (piece->getRow() == newRow && piece->getCol() == newCol &&
                piece->isWhite != isWhite) {
                return true;
            }
        }
    }

        // Check for En Passant
        if (std::abs(newCol - col) == 1 && newRow == row + direction) {
            for (Piece* piece : pieces) {
                if (piece->getRow() == row && piece->getCol() == newCol &&
                    piece->isWhite != isWhite && piece->twoStepped) {
                    return true;
                }
            }
        }
    return false;
}
