#include "load_game_command.h"
#include "game.h"
#include <iostream>

void LoadGameCommand::execute(Game& game) {
    if (game.loadFromFile(filename)) {
        std::cout << "Game loaded from " << filename << "\n";
    } else {
        std::cout << "Failed to load game: " << game.getLastError() << "\n";
    }
}
