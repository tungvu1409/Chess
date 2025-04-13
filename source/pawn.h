#ifndef PAWN_H_INCLUDED
#define PAWN_H_INCLUDED

#include "piece.h"

class Pawn : public Piece {
public:
    Pawn(int row, int col, bool isWhite);
    bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) override;


    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
