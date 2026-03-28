#include "game.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        Game game;
        game.run();
        return 0;
    } 
    catch (const std::exception& e) {
        std::cout << "Fatal error: " << e.what() << std::endl;
        std::cout << "The game will now close." << std::endl;
        return 1;
    }
    catch (...) {
        std::cout << "Unknown fatal error occurred." << std::endl;
        std::cout << "The game will now close." << std::endl;
        return 1;
    }
}