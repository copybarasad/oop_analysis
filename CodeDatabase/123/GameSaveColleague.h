#ifndef GAME_SAVE_COLLEAGUE
#define GAME_SAVE_COLLEAGUE

#include "IGameSaveMediator.h"

class GameSaveColleague
{
protected:
    IGameSaveMediator *mMediator;

public:
    virtual void setMediator(IGameSaveMediator *mediator) = 0;
};

#endif