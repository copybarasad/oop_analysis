#ifndef GAME_COLLEAGUE
#define GAME_COLLEAGUE

// #include "IGameSaveMediator.h"
class GameSaveMediator;

class GameColleague
{

public:
    virtual void setMediator(GameSaveMediator *mediator) = 0;
};

#endif