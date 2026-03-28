#include "save_game_command.h"
#include "game.h"
#include <iostream>

void SaveGameCommand::execute(Game& game) {
    if (game.saveToFile(filename)) {
        std::cout << "Game saved to " << filename << "\n";
    } else {
        std::cout << "Failed to save game: " << game.getLastError() << "\n";
    }
}
