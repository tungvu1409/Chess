#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED
#include <vector>

struct Piece {
    int row, col;
    bool isWhite;  // Biến isWhite để xác định quân cờ trắng hay đen
    int type;      // Để xác định loại quân cờ (Vua, Hậu, Xe, ...)

    Piece(int r, int c, bool white, int t)
        : row(r), col(c), isWhite(white), type(t) {}

    // Getter methods
    int getRow() const { return row; }
    int getCol() const { return col; }
    int getType() const { return type; }

    // Setter methods
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }

    // Di chuyển quân cờ
    void move(int newRow, int newCol) {
        row = newRow;
        col = newCol;
    }

    // Phương thức ảo kiểm tra nước đi hợp lệ, được override trong các lớp con
    virtual bool isValidMove(int newRow, int newCol, std::vector<Piece*>& pieces) {
        return false; // Lớp cơ sở trả về false mặc định
    }


};

#endif

