#pragma once
#include "Command.h"

class UpgradeDamageCommand : public Command {
public:
    void execute(GameCycle& game) override {
        game.upgradeDamage();
        game.proceedToNextLevel();
    }
};
