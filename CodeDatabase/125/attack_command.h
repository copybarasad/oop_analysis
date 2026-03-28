#pragma once
#include "command.h"

class AttackCommand : public Command {
public:
    void Execute(GameSession& game) override;
    std::string GetDescription() const override { return "Атака"; }
};
