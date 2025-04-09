#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece {
public:
    // Constructor của quân Vua, gọi constructor của lớp Piece
    King(int r, int c, bool white) : Piece(r, c, white, 0) {}

    // Hàm kiểm tra tính hợp lệ của nước đi của quân Vua
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
