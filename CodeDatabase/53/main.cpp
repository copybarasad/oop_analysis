#include <iostream>
#include <windows.h>
#include <clocale>
#include "Tests.h"
#include "Game.h"
#include "GameController.h"
#include <ctime>

using namespace std;




int main() {
    
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleOutputCP(CP_UTF8);
    std::srand(std::time(0));
    
    try {
        Game game;
        
        auto& controller = game.getGameControl();

        controller.showMainMenu();
        
        std::cout << "\nСпасибо за игру! До свидания!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n=== КРИТИЧЕСКАЯ ОШИБКА ===" << std::endl;
        std::cerr << "Причина: " << e.what() << std::endl;
        std::cerr << "Игра будет закрыта." << std::endl;
        return 1;
    }
    return 0;
}