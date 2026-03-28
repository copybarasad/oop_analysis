#pragma once
#include "command.h"

class MoveCommand : public Command {
public:
    enum class Direction { Up, Down, Left, Right };
    
    MoveCommand(Direction direction);
    void Execute(GameSession& game) override;
    std::string GetDescription() const override;
    
private:
    Direction direction_;
};
