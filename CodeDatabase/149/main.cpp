#include <iostream>
#include <memory>
#include <cassert>
#include "Game.h"
int main() {
   try {
        Game game;
        game.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}