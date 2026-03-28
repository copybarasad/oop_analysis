#include "MovementCommand.h"

MovementCommand::MovementCommand(CommandType commandType, int dx, int dy) 
    : type(commandType), deltaX(dx), deltaY(dy) {}

CommandType MovementCommand::getType() const {
    return type;
}

bool MovementCommand::isValid() const {
    return type == CommandType::MOVE_UP || type == CommandType::MOVE_DOWN ||
           type == CommandType::MOVE_LEFT || type == CommandType::MOVE_RIGHT;
}

int MovementCommand::getDeltaX() const {
    return deltaX;
}

int MovementCommand::getDeltaY() const {
    return deltaY;
}