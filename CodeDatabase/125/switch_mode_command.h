#pragma once
#include "command.h"

class SwitchModeCommand : public Command {
public:
    void Execute(GameSession& game) override;
    std::string GetDescription() const override { return "Смена режима боя"; }
};
