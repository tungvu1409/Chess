#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
enum GameMode {
    MODE_NONE,
    MODE_PLAYER_VS_PLAYER,
    MODE_PLAYER_VS_AI
};


struct graphic
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;


    void init();
    void run();
    TTF_Font* font = nullptr;
    GameMode showMenu();

    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y);
};

#endif
