#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece {
public:
    // Constructor của quân Xe, gọi constructor của lớp Piece
    Rook(int r, int c, bool white) : Piece(r, c, white, 2) {}

    // Hàm kiểm tra tính hợp lệ của nước đi của quân Xe
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
