#ifndef KNIGHT_H_INCLUDED
#define KNIGHT_H_INCLUDED

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(int row, int col, bool isWhite);
    bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) override;

    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
