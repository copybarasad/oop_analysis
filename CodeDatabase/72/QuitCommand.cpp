//
// Created by Mac on 02.12.2025.
//

#include "QuitCommand.h"

#include <iostream>

CommandType QuitCommand::getType() const {
    return CommandType::QUIT_GAME;
}

CommandResult QuitCommand::execute(Game &game) {
    return CommandResult::exit(true);
}

