#ifndef LOAD_GAME
#define LOAD_GAME

#include "GoToGame.h"
#include "../../SaveSystem/SaveSystem.h"
#include "../../GameSystem/GameSaveMediator.h"

template <typename T>
class LoadGame : public GoToGame<T>
{
private:
    SaveSystem mSave;
    GameSaveMediator mMediator;

public:
    LoadGame(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game) : GoToGame<T>(screen, controller, game)
    {
        mMediator.setComponent(this->mGame);
        mMediator.setComponent(&mSave);
    }

    bool execute() override
    {
        mSave.load();
        this->mGame->putEverythingOnTheMap();

        return GoToGame<T>::execute();
    }
};

#endif