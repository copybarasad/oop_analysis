#include "IO/controller.hpp"
#include "IO/gameview.hpp"
#include "managers/gamemanager.hpp"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Dungeon Adventure Game ===\n";
    
    std::unique_ptr<Game> game;
    std::cout << "1. New Game\n2. Load Game\nChoice: ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "2") {
        std::cout << "Enter save file name: ";
        std::string filename;
        std::getline(std::cin, filename);
        game = std::make_unique<Game>(filename);
    } else {
        game = std::make_unique<Game>(20, 20);
    }
    
    GameController<> controller(game.get(), "game_config.json");
    GameView<> view(game.get());
    
    controller.setOnGameStateChanged([&view]() {
        view.render();
    });
    
    view.render();
    controller.runGameLoop();
    
    view.render();
    if (game->isGameOver()) {
        view.renderMessage("Game Over!");
    } else if (game->playerWon()) {
        view.renderMessage("You Won!");
    }
   
    return 0;
}