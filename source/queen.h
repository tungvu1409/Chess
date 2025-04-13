#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece {
public:
    Queen(int row, int col, bool isWhite);
    bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) override;
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
