#include "graphic.h"
#include <iostream>
#include "board.h"

void graphic::init(){
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Unable to initialize SDL! Error: " << SDL_GetError() << std::endl;

    }
    window = SDL_CreateWindow("Chess",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              800, 600,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Could not create window! Error: " << SDL_GetError() << std::endl;

    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "COuld not create renderer! Error: " << SDL_GetError() << std::endl;
    }

}

void graphic::run() {
    bool running = true;
    SDL_Event event;
    Board board;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(16);
        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Vẽ bàn cờ
        board.draw(renderer);

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
