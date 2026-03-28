#include <iostream>
#include <ctime>
#include "Game.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
    Game game;
    game.start();
    return 0;
}