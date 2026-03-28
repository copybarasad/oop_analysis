#ifndef MOVELEFTCOMMAND_H
#define MOVELEFTCOMMAND_H

#include "Command.h"

class MoveLeftCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override { return "MoveLeft"; }
    Command* clone() const override { return new MoveLeftCommand(*this); }
};

#endif