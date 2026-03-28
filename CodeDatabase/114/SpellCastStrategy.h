#ifndef SPELL_CAST_STRATEGY_H
#define SPELL_CAST_STRATEGY_H

#include "IStrategy.h"
#include <limits>
#include <cstddef>

class PlayerController;
class HandController;
class InputController;

class SpellCastStrategy : public IInputStrategy {
private:
    PlayerController* playerController;
    HandController* handController;
    InputController* inputController;
public:
    SpellCastStrategy(PlayerController* playerController, HandController* handController, InputController* inputController);
    bool handle() override;
};

#endif

