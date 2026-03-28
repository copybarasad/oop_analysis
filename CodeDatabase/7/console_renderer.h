#pragma once
#include <iostream>
#include <string>
#include <vector>

class Game;

class ConsoleRenderer {
public:
    ConsoleRenderer() {} 
    
    void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }
    
    void render(const Game& game);
    void renderMessage(const std::string& message) {
        std::cout << message << std::endl;
    }
    
    void showPrompt(const std::string& prompt) {
        std::cout << prompt;
        std::cout.flush();
    }
};