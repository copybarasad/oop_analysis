//
// Created by Mac on 02.12.2025.
//

#include "UnknownCommand.h"

UnknownCommand::UnknownCommand(const char &ch)
    :inputChar(ch){}

CommandType UnknownCommand::getType() const {
    return CommandType::UNKNOWN;
}

CommandResult UnknownCommand::execute(Game &game) {
    return CommandResult::failure("Unknown command!");
}



