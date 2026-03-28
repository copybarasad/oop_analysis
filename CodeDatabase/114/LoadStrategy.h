#ifndef LOAD_STRATEGY_H
#define LOAD_STRATEGY_H

#include "IStrategy.h"

class Game;
class InputController;

class LoadStrategy : public IInputStrategy {
private:
    Game* game;
    InputController* inputController;
public:
    LoadStrategy(Game* game, InputController* inputController);
    bool handle() override;
};

#endif

