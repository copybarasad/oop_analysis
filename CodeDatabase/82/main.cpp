#include "GameManager.h"
#include "GameException.h"
#include <iostream>

int main() {
    try {
        GameManager gameManager;
        gameManager.run();
    } catch (const GameException& e) {
        std::cerr << "Критическая ошибка в игре: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) { 
        std::cerr << "Неизвестная критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}