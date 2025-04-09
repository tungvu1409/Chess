#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece {
public:
    // Constructor của quân Mã, gọi constructor của lớp Piece
    Knight(int r, int c, bool white) : Piece(r, c, white, 4) {}

    // Hàm kiểm tra tính hợp lệ của nước đi của quân Mã
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
