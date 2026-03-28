#ifndef GO_TO_SCREEN
#define GO_TO_SCREEN

#include "ICommand.h"
#include "../Controllers/LogicControllers/IController.h"
#include "../../Screens/IScreen.h"
#include "../../Screens/Screens.h"
#include "../../GameSystem/Game.h"
// #include "../../Screens/PauseScreen.h"
// #include "../Controllers/LogicControllers/PauseController.h"
// #include "../../Screens/GameScreen.h"
// #include "../../Controllers/LogicControllers/GameController.h"

template <typename T>
class GoToScreen : public ICommand
{

protected:
    shared_ptr<IScreen> *mScreen;
    shared_ptr<IController<T>> *mController;
    Screens<T> mScreens;
    Game *mGame;
    // pair<shared_ptr<IScreen> *, shared_ptr<IController<T>> *> mState;
    // shared_ptr<IScreen> mNewScreen;
    // Room mRoom;

    // static map<Room, shared_ptr<IScreen>> mScreens;

public:
    GoToScreen(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game)
    {
        mScreen = screen;
        mController = controller;
        mGame = game;
    }
    // GoToScreen(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, pair<shared_ptr<IScreen> *, shared_ptr<IController<T>> *> state)
    // {
    //     mScreen = screen;
    //     mController = controller;
    //     mState = state;
    // }

    // bool execute() override
    // {
    //     *this->mScreen = nullptr;
    //     *this->mController = nullptr;
    //     *this->mScreen = *mState.first;
    //     *this->mController = *mState.second;
    //     // *mScreen = make_shared<PauseScreen>();
    //     return true;
    // }
};

#endif