#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(int row, int col, bool isWhite);
    bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) override;

    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif

