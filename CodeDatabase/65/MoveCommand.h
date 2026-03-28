#pragma once

#include "ICommand.h"
#include "InputManager.h"  // Используем Direction из InputManager

class MoveCommand : public ICommand {
private:
    Direction direction;

public:
    explicit MoveCommand(Direction dir) : direction(dir) {}
    
    void execute(Game& game) override;
    Direction getDirection() const { return direction; }
};
