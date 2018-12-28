#include "Game.h"

int main() {
    Game game;

    bool initialized = game.initialize();

    if(!initialized) {
        SDL_Log("Failed to initialize game");
        return -1;
    }

    return game.run();
}