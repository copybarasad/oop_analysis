#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <exception>

int main() {
    try {
        Game game;
        game.startGame(); 
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Неизвестная критическая ошибка!" << std::endl;
        return 1;
    }

    return 0;
}