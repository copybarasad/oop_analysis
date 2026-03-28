#ifndef SWITCHMODECOMMAND_H
#define SWITCHMODECOMMAND_H

#include "Command.h"

class SwitchModeCommand : public Command {
public:
    void execute(Game& game) override;
    std::string getName() const override { return "SwitchMode"; }
    Command* clone() const override { return new SwitchModeCommand(*this); }
};

#endif