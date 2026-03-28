#include "game.h"
#include <iostream>

int main() {
    auto [isNewGame, filename] = Game::displayMainMenu();
    Game* gamePtr = nullptr;
    if (isNewGame) {
        gamePtr = new Game(20, 20, 5);
    } else {
        gamePtr = new Game(20, 20, 5);
        if (!gamePtr->loadGame(filename)) {
            std::cout << "Field to load game. Starting a new game.\n";
            delete gamePtr;
            gamePtr = new Game(20, 20, 5);
        }
    }
    gamePtr->run();
    delete gamePtr;
    return 0;
}