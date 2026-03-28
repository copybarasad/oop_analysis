#ifndef ATTACK_STRATEGY_H
#define ATTACK_STRATEGY_H

#include "IStrategy.h"

class PlayerController;
class InputController;

class AttackStrategy : public IInputStrategy {
private:
    PlayerController* playerController;
    InputController* inputController;
public:
    AttackStrategy(PlayerController* playerController, InputController* inputController);
    bool handle() override;
};

#endif

