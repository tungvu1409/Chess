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
                              800, 600,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Could not create window! Error: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Could not create renderer! Error: " << SDL_GetError() << std::endl;
    }

    if (TTF_Init() < 0)
    {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("run/times.ttf", 48);
    if (!font)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags)
    {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
    }
    backgroundTexture = IMG_LoadTexture(renderer, "run/background1.png");
    if (!backgroundTexture) {
        std::cerr << "Failed to load background: " << IMG_GetError() << std::endl;
    }


    // am thanh

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    // am thanh o menu
    menuMusic = Mix_LoadMUS("run/menu.wav");
    if (!menuMusic) {
        std::cerr << "Failed to load menu music: " << Mix_GetError() << std::endl;
    } else {
        Mix_PlayMusic(menuMusic, -1);  // Fade in trong 2 giây
    }
    // am thanh khi an nut start
    startSound = Mix_LoadWAV("run/start.wav");
    if (!startSound) {
        std::cerr << "Failed to load move.wav: " << Mix_GetError() << std::endl;
    }
    // am thanh di chuyen quan co
    moveSound = Mix_LoadWAV("run/move.wav");
    if (!moveSound) {
        std::cerr << "Failed to load move.wav: " << Mix_GetError() << std::endl;
    }
    // am thanh khi nguoi choi chien thang
    winSound = Mix_LoadWAV("run/winner.wav");
    if (!winSound) {
        std::cerr << "Failed to load win sound: " << Mix_GetError() << std::endl;
    }
}
void graphic::playMoveSound() {
    if (moveSound) {
        Mix_PlayChannel(-1, moveSound, 0);
    }
}
void graphic::run()
{
    bool running = true;
    SDL_Event event;
    Board board(*this);  // Truyền graphic vào

    board.init(renderer);

    MenuResult menuResult = showMenu();
    if (menuResult.mode == MODE_NONE) return;
    Mix_HaltMusic();                  // Dừng nhạc menu
    Mix_FreeMusic(menuMusic);        // Giải phóng nhạc
    menuMusic = nullptr;

    board.whiteTimeLeft = menuResult.timeLimit;
    board.blackTimeLeft = menuResult.timeLimit;
    Uint32 lastTick = SDL_GetTicks(); // Thời điểm frame trước
    Uint32 lastSecondTick = SDL_GetTicks(); // Thời gian từ giây trước
    bool gameOverNotified = false;  // Cờ để thông báo hết giờ chỉ một lần

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

        SDL_RenderClear(renderer);

        if (backgroundTexture) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr); // Vẽ background
        }

        // Kiểm tra xem một giây đã trôi qua chưa
        Uint32 currentTick = SDL_GetTicks();
        Uint32 deltaTime = currentTick - lastTick;
        lastTick = currentTick;

        // Giảm thời gian mỗi giây (1 giây trôi qua)
        if (currentTick - lastSecondTick >= 1000) {
            if (!board.gameOver) {
                if (board.turnIsWhite) {
                    board.whiteTimeLeft -= 1; // Giảm 1 giây cho bên trắng
                } else {
                    board.blackTimeLeft -= 1; // Giảm 1 giây cho bên đen
                }
                lastSecondTick = currentTick; // Cập nhật thời gian của giây tiếp theo
            }
        }

        // Kiểm tra hết giờ
        if ((board.whiteTimeLeft <= 0 || board.blackTimeLeft <= 0) && !gameOverNotified) {
            board.gameOver = true;
            gameOverNotified = true; // Đánh dấu là đã thông báo hết giờ

            std::cout << "Hết giờ! ";
            std::cout << (board.whiteTimeLeft <= 0 ? "Black wins!" : "White wins!") << std::endl;
        }

        board.draw(renderer);

        // Hiển thị "Check!" nếu có
        if (board.isInCheck(board.turnIsWhite)) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 180);
            SDL_Rect rect = { 620, 250, 160, 60 };
            SDL_RenderFillRect(renderer, &rect);

            SDL_Color white = {255, 255, 255, 255};

            int textW, textH;
            TTF_SizeText(font, "Check!", &textW, &textH);
            int textX = rect.x + (rect.w - textW) / 2;
            int textY = rect.y + (rect.h - textH) / 2;

            renderText(renderer, font, "Check!", white, textX, textY);
        }

        // Hiển thị đồng hồ
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color black = {0, 0, 0, 255};


        int minutesW = board.whiteTimeLeft / 60;
        int secondsW = board.whiteTimeLeft % 60;
        std::string timeW = std::to_string(minutesW) + ":" + (secondsW < 10 ? "0" : "") + std::to_string(secondsW);
        renderText(renderer, font, timeW, white, 655, 5);

        int minutesB = board.blackTimeLeft / 60;
        int secondsB = board.blackTimeLeft % 60;
        std::string timeB =  std::to_string(minutesB) + ":" + (secondsB < 10 ? "0" : "") + std::to_string(secondsB);
        renderText(renderer, font, timeB, black, 655, 550);

        // Nếu hết giờ và thông báo đã được kích hoạt, hiển thị thông báo "Game Over"
        if (board.gameOver) {
            SDL_Color red = {255, 0, 0, 255}; // Màu đỏ cho thông báo
            std::string gameOverMessage = (board.whiteTimeLeft <= 0) ? "Black wins!" : "White wins!";
            renderText(renderer, font, gameOverMessage, red, 200, 300);
            if (Mix_Playing(-1) == 0) { // Nếu không có âm nào đang phát
                Mix_PlayChannel(-1, winSound, 0);
            }

        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // Delay để giới hạn FPS
    }

    // Giải phóng tài nguyên âm thanh
    Mix_FreeChunk(moveSound);
    Mix_CloseAudio();




    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}



MenuResult graphic::showMenu()
{
    bool inMenu = true;
    SDL_Event event;
    GameMode selectedMode = MODE_NONE;
    int timeLimit = 300; // mặc định 5 phút (tính bằng giây)

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "run/background.jpg");
    if (!backgroundTexture)
    {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
    }

    SDL_Rect btnPvP = {200, 250, 200, 50}; // Nút Start
    SDL_Rect btnTime = {200, 320, 200, 40}; // Nút chỉnh giờ

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color shadowColor = {0, 0, 0, 255};

    bool isButtonPressed = false;

    TTF_Font* largeFont = TTF_OpenFont("run/times.ttf", 120);
    if (!largeFont)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return {MODE_NONE, 0};
    }

    TTF_Font* regularFont = TTF_OpenFont("run/times.ttf", 30);
    if (!regularFont)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return {MODE_NONE, 0};
    }

    const char* titleText = "CHESS";
    int titleW, titleH;
    TTF_SizeText(largeFont, titleText, &titleW, &titleH);
    int titleX = 300 - titleW / 2;
    int titleY = 50;

    Uint8 alpha = 0;
    bool fadingOut = false;

    while (inMenu)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return {MODE_NONE, 0};

            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x, y = event.button.y;
                if (x >= btnPvP.x && x <= btnPvP.x + btnPvP.w &&
                    y >= btnPvP.y && y <= btnPvP.y + btnPvP.h)
                {
                    isButtonPressed = true;

                    SDL_RenderClear(renderer);
                    if (backgroundTexture)
                        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

                    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
                    SDL_RenderFillRect(renderer, &btnPvP);

                    int textW, textH;
                    TTF_SizeText(regularFont, "Start", &textW, &textH);
                    int textX = btnPvP.x + (btnPvP.w - textW) / 2;
                    int textY = btnPvP.y + (btnPvP.h - textH) / 2;
                    renderText(renderer, regularFont, "Start", white, textX, textY);
                    SDL_RenderPresent(renderer);

                    if (startSound)
                        Mix_PlayChannel(-1, startSound, 0);

                    SDL_Delay(300);
                    fadingOut = true;
                }
                else if (x >= btnTime.x && x <= btnTime.x + btnTime.w &&
                         y >= btnTime.y && y <= btnTime.y + btnTime.h)
                {
                    if (timeLimit == 300)
                        timeLimit = 600;
                    else if (timeLimit == 600)
                        timeLimit = 900;
                    else if (timeLimit == 900)
                        timeLimit =1800;
                    else
                        timeLimit = 300; // quay về 5 phút
                }
            }
        }

        SDL_RenderClear(renderer);
        if (backgroundTexture)
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        renderText(renderer, largeFont, titleText, shadowColor, titleX + 3, titleY + 3);
        renderText(renderer, largeFont, titleText, white, titleX, titleY);

        // Nút Start
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &btnPvP);
        int textW, textH;
        TTF_SizeText(regularFont, "Start", &textW, &textH);
        int textXBtn = btnPvP.x + (btnPvP.w - textW) / 2;
        int textYBtn = btnPvP.y + (btnPvP.h - textH) / 2;
        renderText(renderer, regularFont, "Start", white, textXBtn, textYBtn);

        // Nút chỉnh giờ
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_RenderFillRect(renderer, &btnTime);

        std::string timeText = "Time: " + std::to_string(timeLimit / 60) + " min";
        TTF_SizeText(regularFont, timeText.c_str(), &textW, &textH);
        int textXTime = btnTime.x + (btnTime.w - textW) / 2;
        int textYTime = btnTime.y + (btnTime.h - textH) / 2;
        renderText(renderer, regularFont, timeText.c_str(), white, textXTime, textYTime);

        // Fade-out
        if (fadingOut)
        {
            alpha += 5;
            if (alpha > 255) alpha = 255;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
            SDL_RenderFillRect(renderer, NULL);

            SDL_RenderPresent(renderer);
            SDL_Delay(5);

            if (alpha == 255)
            {
                SDL_Delay(1);
                selectedMode = MODE_PLAYER_VS_PLAYER;
                inMenu = false;
            }
        }
        else
        {
            SDL_RenderPresent(renderer);
        }

        SDL_Delay(16);
    }

    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(largeFont);
    TTF_CloseFont(regularFont);

    return {selectedMode, timeLimit};
}




void graphic::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y)
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface)
    {
        std::cerr << "TTF_RenderUTF8_Blended error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}


