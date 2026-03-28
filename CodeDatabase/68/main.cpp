#include <iostream>
#include <memory>
#include "gamemanager.h"
#include "gameexceptions.h"

int main() {
    std::cout << "Spell Battle Game!\n\n";
    
    try {
        GameManager game;
        game.startGame();
    } 
    catch (const SaveGameException& e) {
        std::cout << "Save Error: " << e.what() << "\n";
    }
    catch (const LoadGameException& e) {
        std::cout << "Load Error: " << e.what() << "\n";  
    }
    catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << "\n";
    }
    
    return 0;
}