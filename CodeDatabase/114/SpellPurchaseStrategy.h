#ifndef SPELL_PURCHASE_STRATEGY_H
#define SPELL_PURCHASE_STRATEGY_H

#include "IStrategy.h"

class PlayerController;
class HandController;
class InputController;

class SpellPurchaseStrategy : public IInputStrategy {
private:
    PlayerController* playerController;
    HandController* handController;
    InputController* inputController;
public:
    SpellPurchaseStrategy(PlayerController* playerController, HandController* handController, InputController* inputController);
    bool handle() override;
};

#endif

