#include "board.h"


void Board::draw(SDL_Renderer* renderer) {
    int tileSize = 75;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 107, 142, 35, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 227, 218, 201, 255);
            }

            SDL_Rect rect = { col * tileSize, row * tileSize, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
