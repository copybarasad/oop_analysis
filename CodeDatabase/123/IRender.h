#ifndef IRENDERER
#define IRENDERER

#include "../GameSystem/Game.h"
#include "../Screens/Screens.h"
#include "../Screens/IScreen.h"

class IRender
{
protected:
    shared_ptr<IScreen> mCurrentScreen;
    shared_ptr<Game> mGame;
    // shared_ptr<SaveSystem> mSaveSystem;

public:

    void setGame(shared_ptr<Game> game)
    {
        mGame = game;
    }

    shared_ptr<IScreen> *getCurrentScreen()
    {
        return &mCurrentScreen;
    }

    virtual bool isEnd() = 0;

    virtual void start() = 0;
};

#endif