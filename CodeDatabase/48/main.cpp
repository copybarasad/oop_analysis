#include <iostream>
#include <memory>
#include <limits>
#include "lib/Game.h"

#ifdef _WIN32
#include <windows.h>
#endif

void setupConsole() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
}

int getInputInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail() || value < min || value > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number between " << min << " and " << max << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

int main() {
    setupConsole();
    
    Game game;
    game.start();
    
    std::cout << "\nThanks for playing! Press Enter to exit...";
    std::cin.get();
    
    return 0;
}