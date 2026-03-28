#ifndef MOVEDOWNCOMMAND_H
#define MOVEDOWNCOMMAND_H

#include "Command.h"

class MoveDownCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override { return "MoveDown"; }
    Command* clone() const override { return new MoveDownCommand(*this); }
};

#endif