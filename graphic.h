#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL.h>

struct graphic {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;


    void init();
    void run();

};

#endif
