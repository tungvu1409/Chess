#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece {
public:
    // Constructor của quân Hậu, gọi constructor của lớp Piece
    Queen(int r, int c, bool white) : Piece(r, c, white, 1) {}

    // Hàm kiểm tra tính hợp lệ của nước đi của quân Hậu
    bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) override;
};

#endif
