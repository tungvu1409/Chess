#ifndef ROOK_H
#define ROOK_H
#include <vector>
#include "Piece.h"

class Rook : public Piece {
public:
    Rook(int row, int col, bool isWhite);
    bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) override;

    bool isValidMove(int newRow, int newCol,std::vector<Piece*>& pieces) override;
};

#endif
