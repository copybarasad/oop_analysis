#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "Command.h"

class MoveCommand : public Command {
private:
    int dx, dy;
public:
    MoveCommand(int deltaX, int deltaY);
    bool execute(GameState& gameState) override;
    std::string getName() const override;
};

#endif 