#pragma once

#include "ICommand.h"

class ChangeAttackModeCommand : public ICommand {
public:
    void execute(Game& game) override;
};
