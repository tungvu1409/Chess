#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL.h>

struct graphic {
    SDL_Window* window = nullptr;



    void init();
    void run();

};

#endif // GRAPHIC_H_INCLUDED
