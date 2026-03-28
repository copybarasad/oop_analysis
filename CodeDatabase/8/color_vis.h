#pragma once
#include <iostream>
#include <string>
#include <vector> 

class Game;
class Field;
class Node;

class ColorfulRenderer {
private:
    std::vector<std::string> messageBuffer;
    
    const std::string RED = "\033[91m";
    const std::string GREEN = "\033[92m";
    const std::string YELLOW = "\033[93m";
    const std::string BLUE = "\033[94m";
    const std::string PURPLE = "\033[95m";
    const std::string CYAN = "\033[96m";
    const std::string RESET = "\033[0m";

public:
    ColorfulRenderer() {} 
    
    void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }
    
    void render(const Game& game);
    void renderMessage(const std::string& message);
    void showPrompt(const std::string& prompt);
};