#ifndef ROOK_H
#define ROOK_H
#include <vector>
#include "Piece.h"

class Rook : public Piece {
public:
    bool hasMoved = false;
    Rook(int row, int col, bool isWhite);
    void move(int newRow, int newCol) override {
        row = newRow;
        col = newCol;
        hasMoved = true;  // Đánh dấu xe đã di chuyển
    }
    bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) override;
    bool isValidMove(int newRow, int newCol,std::vector<Piece*>& pieces) override;
};

#endif
