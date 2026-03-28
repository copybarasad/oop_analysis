#include <iostream>
#include "Game.h"
#include "Gamefield.h"

int main() {

    try {
        Game game;

        game.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Game finished\n";
    return 0;
}
