#include "MainGame.h"
#include <iostream>

int main() {
    std::srand(std::time(0));

    try {
        MainGame game;
        game.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}