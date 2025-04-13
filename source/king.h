#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece {
public:
    King(int row, int col, bool isWhite);

    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif

