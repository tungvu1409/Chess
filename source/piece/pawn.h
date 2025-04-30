#ifndef PAWN_H_INCLUDED
#define PAWN_H_INCLUDED

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(int row, int col, bool isWhite);
    bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) override;

    void move(int newRow, int newCol) override;
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
