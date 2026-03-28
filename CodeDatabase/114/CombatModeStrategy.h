#ifndef COMBAT_MODE_STRATEGY_H
#define COMBAT_MODE_STRATEGY_H

#include "IStrategy.h"

class PlayerController;

class CombatModeStrategy : public IInputStrategy {
private:
    PlayerController* playerController;
public:
    CombatModeStrategy(PlayerController* playerController);
    bool handle() override;
};

#endif

