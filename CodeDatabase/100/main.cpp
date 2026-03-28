#include <iostream>
#include "../game/game_main/Game.h"
#include "../game/interface/ConsoleRenderer.h"

int main() {

    try {
        Game game(15, 10, 100, 20);
        game.showMainMenu();
    } 
    catch (const std::exception& e) {
        std::cerr << Colors::RED << "Fatal error: " << e.what() << std::endl;
        std::cerr << "Game cannot be started. Press ENTER to exit..." << Colors::RESET;
        std::cin.ignore(10000, '\n');
        return 1; 
    }
    
    return 0;
}