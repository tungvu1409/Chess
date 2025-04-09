#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {
public:
    // Constructor của quân Tượng, gọi constructor của lớp Piece
    Bishop(int r, int c, bool white) : Piece(r, c, white, 3) {}

    // Hàm kiểm tra tính hợp lệ của nước đi của quân Tượng
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
