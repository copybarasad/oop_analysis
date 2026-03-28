#ifndef UPDATE_PLAYER
#define UPDATE_PLAYER

#include "ICommand.h"
#include "GoToGame.h"
#define UPGRADE_COST 40

template <typename T>
class UpdatePlayer : public ICommand
{
protected:
    Game *mGame;
    shared_ptr<IScreen> *mScreen;
    shared_ptr<IController<T>> *mController;

public:
    UpdatePlayer(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game)
    {
        mScreen = screen;
        mController = controller;
        mGame = game;
    }
};

#endif