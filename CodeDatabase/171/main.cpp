#include <iostream>
#include <memory>
#include "game.h"

int main() {
    std::setlocale(LC_ALL, "Russian");

    try {
        Game game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Спасибо за игру! До свидания!" << std::endl;
    return 0;
}