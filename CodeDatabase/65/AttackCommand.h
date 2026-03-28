#pragma once

#include "ICommand.h"

class AttackCommand : public ICommand {
public:
    void execute(Game& game) override;
};
