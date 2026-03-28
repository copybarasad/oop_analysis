#ifndef TRANSITION_TO_NEW_LEVEL_SCREEN
#define TRANSITION_TO_NEW_LEVEL_SCREEN

#include "IScreen.h"
#include "../HasFont.h"
#include "../GameSystem/Game.h"

class TransitionToNewLevelScreen : public IScreen, public HasFont
{
protected:
    Game *mGame;

public:
    TransitionToNewLevelScreen(Game *game)
    {
        mGame = game;
    }
    void tick() override {}
};

#endif