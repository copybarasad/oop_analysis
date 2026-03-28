//
// Created by Mac on 02.12.2025.
//

#include "MoveCommand.h"

MoveCommand::MoveCommand(const Direction &dir)
    :direction(dir) {}

CommandType MoveCommand::getType() const {
    switch(direction) {
        case Direction::Up: return CommandType::MOVE_UP;
        case Direction::Down: return CommandType::MOVE_DOWN;
        case Direction::Left: return CommandType::MOVE_LEFT;
        case Direction::Right: return CommandType::MOVE_RIGHT;
        default: return CommandType::UNKNOWN;
    }
}

CommandResult MoveCommand::execute(Game &game) {
    auto field = game.getField();
    auto movement = std::make_unique<MovementController>(field);
    bool canMove = movement->movePlayer(direction);

    if (!canMove) {
        return CommandResult::failure("You can't move!");
    }

    return CommandResult::success();
}



