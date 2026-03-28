#ifndef MOVERIGHTCOMMAND_H
#define MOVERIGHTCOMMAND_H

#include "Command.h"

class MoveRightCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override { return "MoveRight"; }
    Command* clone() const override { return new MoveRightCommand(*this); }
};

#endif