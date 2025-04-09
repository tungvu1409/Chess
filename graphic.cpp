#include "graphic.h"
#include <iostream>
#include "board.h"

void graphic::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Unable to initialize SDL! Error: " << SDL_GetError() << std::endl;

    }
    window = SDL_CreateWindow("Chess",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              600, 600,
                              SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "Could not create window! Error: " << SDL_GetError() << std::endl;

    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "COuld not create renderer! Error: " << SDL_GetError() << std::endl;
    }
    if (TTF_Init() < 0){
    std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("times.ttf", 48);
    if (!font){
    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

}

void graphic::run()
{
    bool running = true;
    SDL_Event event;
    Board board;
    GameMode mode = showMenu();
    if (mode == MODE_NONE) return;
    board.init(renderer);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                board.handleMouseClick(x, y);
            }

        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        board.draw(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
GameMode graphic::showMenu()
{
    bool inMenu = true;
    SDL_Event event;
    GameMode selectedMode = MODE_NONE;

    SDL_Rect btnPvP = {200, 250, 200, 50};
    SDL_Rect btnPvAI = {200, 330, 200, 50};

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color btnColor = {100, 100, 255, 255};
    SDL_Color bgColor = {30, 30, 30, 255};

    while (inMenu)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return MODE_NONE;
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x, y = event.button.y;
                if (x >= btnPvP.x && x <= btnPvP.x + btnPvP.w &&
                    y >= btnPvP.y && y <= btnPvP.y + btnPvP.h)
                {
                    selectedMode = MODE_PLAYER_VS_PLAYER;
                    inMenu = false;
                }
                else if (x >= btnPvAI.x && x <= btnPvAI.x + btnPvAI.w &&
                         y >= btnPvAI.y && y <= btnPvAI.y + btnPvAI.h)
                {
                    selectedMode = MODE_PLAYER_VS_AI;
                    inMenu = false;
                }
            }
        }

        // Vẽ nền
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);

        // Vẽ tiêu đề
        renderText(renderer, font, "CHESS GAME", white, 180, 100);

        // Vẽ nút
        SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b, btnColor.a);
        SDL_RenderFillRect(renderer, &btnPvP);
        SDL_RenderFillRect(renderer, &btnPvAI);

        renderText(renderer, font, "Chơi với người", white, btnPvP.x + 25, btnPvP.y + 12);
        renderText(renderer, font, "Chơi với máy", white, btnPvAI.x + 35, btnPvAI.y + 12);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return selectedMode;
}

void graphic::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y)
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "TTF_RenderUTF8_Blended error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}



