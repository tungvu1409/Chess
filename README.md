# **♟️ Chess Game in C++ with SDL2**
![image](https://github.com/user-attachments/assets/712e4714-f7fa-4117-8956-f66d1a6b6a38)

## **📝 Overview**
This is a simple two-player chess game built with C++ and SDL2. It provides a graphical interface for local player-vs-player matches, following standard chess rules.
## **⚙️ Prerequisites**
Before you begin, ensure you have the following installed and ready:

  • C++ compiler (e.g., g++ or MinGW)

  • SDL2 (for graphics)

  • SDL2_image (for loading images)

  • SDL2_ttf (for fonts) and SDL2_mixer (for sounds)
## **🎮 Features**
• Local **Player vs Player** mode

• Full chess rules support:

    • Legal moves for each piece

    • Check and checkmate detection

    • Pawn promotion

    • Castling

    • En passant

    • Stalemate

• Simple menu with options:

    • Start Game

    • Choose Time 

• Graphical interface using SDL2

• Piece textures and board visuals
## **🛠️ Installation**
**Step 1: Install SDL2 Libraries**
Download and install SDL2 and SDL2_image from the official websites.

**Step 2: Compile the Code**
Run the following command to compile the project:
g++ -std=c++17 -Iinclude -Isrc -o chess src/*.cpp -lSDL2 -lSDL2_image

**Step 3: Run the Game**
After compiling, you can run the game by executing:
./chess
## **🎮 Usage**
**Starting the Game**
  • Once the game starts, you will be presented with a simple menu:

  • Start Game – Start a new game between two players.

  • Choose Time – Select the time settings for the game.

**Playing the Game**
  • Move pieces using the mouse (click).

  • Pawn promotion occurs when a pawn reaches the opposite side.

  • Check and Checkmate will be displayed on screen.
