#include "SimpleCommand.h"

SimpleCommand::SimpleCommand(CommandType commandType) : type(commandType) {}

CommandType SimpleCommand::getType() const {
    return type;
}

bool SimpleCommand::isValid() const {
    return type == CommandType::SWITCH_COMBAT_MODE || 
           type == CommandType::BUY_SPELL ||
           type == CommandType::SAVE_GAME ||
           type == CommandType::LOAD_GAME ||
           type == CommandType::QUIT_GAME;
}