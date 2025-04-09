#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece {
public:
    // Constructor của quân Tốt, gọi constructor của lớp Piece
    Pawn(int r, int c, bool white) : Piece(r, c, white, 5) {}

    // Hàm kiểm tra tính hợp lệ của nước đi của quân Tốt
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
