#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece {
public:
    bool hasMoved = false;
    King(int row, int col, bool isWhite);

    void move(int newRow, int newCol) override {
        row = newRow;
        col = newCol;
        hasMoved = true;
    }
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;

};

#endif

