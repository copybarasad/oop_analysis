#ifndef SAVE_STRATEGY_H
#define SAVE_STRATEGY_H

#include "IStrategy.h"

class Game;
class InputController;

class SaveStrategy : public IInputStrategy {
private:
    Game* game;
    InputController* inputController;
public:
    SaveStrategy(Game* game, InputController* inputController);
    bool handle() override;
};

#endif

