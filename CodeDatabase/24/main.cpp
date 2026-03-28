#include "Game.hpp"
#include <iostream>

// Точка входа в программу
int main() {
    // Проверка конфигурации перед запуском
    if (!Config::Constraints::validateAllConstraints()) {
        std::cerr << "Invalid game configuration! Check Configs.h constraints." << std::endl;
        return 1;
    }
    
    try {
        // Создание и запуск игры
        Game game;
        game.run();
    } catch (const std::exception& e) {
        // Обработка ошибок
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
