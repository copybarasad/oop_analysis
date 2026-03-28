#include "QuitCommand.h"
#include <iostream>

bool QuitCommand::execute(GameState& gameState) {
    std::cout << "Quitting game..." << std::endl;
    return true;
}

std::string QuitCommand::getName() const {
    return "QuitCommand";
}