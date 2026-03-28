#pragma once
#include "Command.h"

class SwitchCombatModeCommand : public Command {
public:
    void execute() override;
    std::string getName() const override;
};