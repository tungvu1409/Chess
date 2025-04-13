// piece.h

#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include <vector>

enum PieceType {
    KING = 0,
    QUEEN = 1,
    ROOK = 2,
    BISHOP = 3,
    KNIGHT = 4,
    PAWN = 5
};

class Piece {
public:
    int row, col;
    bool isWhite;
    int type;

    Piece(int row, int col, bool isWhite, int type)
    : row(row), col(col), isWhite(isWhite), type(type) {}


    virtual ~Piece() {}  // ✅ Hàm hủy ảo để tránh warning

    int getRow() const { return row; }
    int getCol() const { return col; }
    int getType() const { return type; }

    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }

    void move(int newRow, int newCol) {
        row = newRow;
        col = newCol;
    }

    virtual bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
        return false;
    }

    virtual bool canMoveTo(int newRow, int newCol, const std::vector<Piece*>& pieces) {
        return false;
    }

    virtual std::vector<std::pair<int, int>> getAllPossibleMoves(const std::vector<Piece*>& pieces) {
        return {};
    }

};

#endif


