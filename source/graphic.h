#ifndef GRAPHIC_H
#define GRAPHIC_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

enum GameMode {
    MODE_NONE,
    MODE_PLAYER_VS_PLAYER,

};

struct MenuResult {
    GameMode mode;
    int timeLimit;

};
struct graphic
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    Mix_Chunk* startSound = nullptr;
    Mix_Chunk* moveSound = nullptr;
    Mix_Chunk* winSound = nullptr;
    Mix_Music* menuMusic = nullptr;
    SDL_Texture* backgroundTexture = nullptr;


    void init();
    void playMoveSound();
    void run();
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y);
    MenuResult showMenu();
};


#endif
