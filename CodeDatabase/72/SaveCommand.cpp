//
// Created by Mac on 02.12.2025.
//

#include "SaveCommand.h"

#include <iostream>

SaveCommand::SaveCommand(const bool &mode)
    :mode(mode){}


CommandType SaveCommand::getType() const {
    return CommandType::SAVE_GAME;
}

CommandResult SaveCommand::execute(Game &game) {
    if (mode) {
        std::cout << "Saving game...\n";
        return CommandResult::exit(true);
    } else {
        return CommandResult::exit(false);
    }
}


