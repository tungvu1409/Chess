#include "ai.h"
#include <limits>
#include <iostream>

AI::AI(bool isWhite) {
    aiIsWhite = isWhite;
}

std::pair<int, int> AI::getBestMove(std::vector<std::vector<Piece*>>& board, int& toRow, int& toCol) {
    int bestFromRow = -1, bestFromCol = -1;
    int bestToRow = -1, bestToCol = -1;
    minimax(board, 3, true, bestFromRow, bestFromCol, bestToRow, bestToCol);  // độ sâu = 3
    toRow = bestToRow;
    toCol = bestToCol;
    return {bestFromRow, bestFromCol};
}

int AI::evaluateBoard(const std::vector<std::vector<Piece*>>& board) {
    int score = 0;
    for (const auto& row : board) {
        for (const auto& piece : row) {
            if (piece != nullptr) {
                int value = 0;
                switch (piece->getType()) {
                    case 'P': value = 10; break;
                    case 'N': value = 30; break;
                    case 'B': value = 30; break;
                    case 'R': value = 50; break;
                    case 'Q': value = 90; break;
                    case 'K': value = 900; break;
                }
                score += piece->isWhite() == aiIsWhite ? value : -value;
            }
        }
    }
    return score;
}

std::vector<std::tuple<int, int, int, int>> AI::generateAllMoves(std::vector<std::vector<Piece*>>& board, bool whiteTurn) {
    std::vector<std::tuple<int, int, int, int>> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board[r][c];
            if (piece != nullptr && piece->isWhite() == whiteTurn) {
                auto legalMoves = piece->getLegalMoves(board);
                for (auto& move : legalMoves) {
                    int toR = move.first;
                    int toC = move.second;
                    moves.push_back({r, c, toR, toC});
                }
            }
        }
    }
    return moves;
}

void AI::makeMove(std::vector<std::vector<Piece*>>& board, int fromRow, int fromCol, int toRow, int toCol, Piece*& captured) {
    captured = board[toRow][toCol];
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = nullptr;
    board[toRow][toCol]->setPosition(toRow, toCol);
}

void AI::undoMove(std::vector<std::vector<Piece*>>& board, int fromRow, int fromCol, int toRow, int toCol, Piece* captured) {
    board[fromRow][fromCol] = board[toRow][toCol];
    board[toRow][toCol] = captured;
    board[fromRow][fromCol]->setPosition(fromRow, fromCol);
}

int AI::minimax(std::vector<std::vector<Piece*>>& board, int depth, bool isMaximizing, int& bestFromRow, int& bestFromCol, int& bestToRow, int& bestToCol) {
    if (depth == 0) {
        return evaluateBoard(board);
    }

    int bestScore = isMaximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    auto moves = generateAllMoves(board, isMaximizing ? aiIsWhite : !aiIsWhite);

    for (auto& move : moves) {
        int fromR, fromC, toR, toC;
        std::tie(fromR, fromC, toR, toC) = move;

        Piece* captured = nullptr;
        makeMove(board, fromR, fromC, toR, toC, captured);

        int dummy1, dummy2, dummy3, dummy4;
        int score = minimax(board, depth - 1, !isMaximizing, dummy1, dummy2, dummy3, dummy4);

        undoMove(board, fromR, fromC, toR, toC, captured);

        if (isMaximizing) {
            if (score > bestScore) {
                bestScore = score;
                bestFromRow = fromR;
                bestFromCol = fromC;
                bestToRow = toR;
                bestToCol = toC;
            }
        } else {
            if (score < bestScore) {
                bestScore = score;
            }
        }
    }

    return bestScore;
}
