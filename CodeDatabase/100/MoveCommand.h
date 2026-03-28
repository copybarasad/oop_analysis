#pragma once
#include "Command.h"

class MoveCommand : public Command {
private:
    int dx, dy;
public:
    MoveCommand(int deltaX, int deltaY);
    void execute() override;
    std::string getName() const override;
    
    int getDX() const;
    int getDY() const;
};