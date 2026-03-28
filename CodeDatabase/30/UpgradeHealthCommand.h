#pragma once
#include "Command.h"

class UpgradeHealthCommand : public Command {
public:
    void execute(GameCycle& game) override {
        game.upgradeHealth();
        game.proceedToNextLevel();
    }
};
