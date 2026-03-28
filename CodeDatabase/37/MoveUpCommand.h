#ifndef MOVEUPCOMMAND_H
#define MOVEUPCOMMAND_H

#include "Command.h"

class MoveUpCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override { return "MoveUp"; }
    Command* clone() const override { return new MoveUpCommand(*this); }
};

#endif