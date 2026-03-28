#ifndef MOVEMENT_STRATEGY_H
#define MOVEMENT_STRATEGY_H

#include "IStrategy.h"

class PlayerController;

class MovementStrategy : public IInputStrategy {
private:
    PlayerController* playerController;
    char input;
public:
    MovementStrategy(PlayerController* playerController, char input);
    bool handle() override;
};

#endif

