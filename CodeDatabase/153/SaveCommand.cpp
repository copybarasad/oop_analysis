#include "SaveCommand.h"
#include "GameSaveSystem.h"
#include <iostream>

SaveCommand::SaveCommand(GameSaveSystem& save)
    : saveSystem(save) {
}

bool SaveCommand::execute(GameState& gameState) {
    try {
        saveSystem.saveGame(gameState);
        std::cout << "Game saved successfully!" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "ERROR saving game: " << e.what() << std::endl;
        return false;
    }
}

std::string SaveCommand::getName() const {
    return "SaveCommand";
}