#ifndef GO_TO_GAME
#define GO_TO_GAME

#include "GoToScreen.h"
#include "../../Screens/GameScreen.h"
// #include "../../Controllers/LogicControllers/GameController.h"

template <typename T>
class GoToGame : public GoToScreen<T>
{
public:
    using GoToScreen<T>::GoToScreen;

    virtual bool execute() override
    {
        *this->mScreen = nullptr;
        *this->mController = nullptr;
        *this->mScreen = this->mScreens.getGameScreen();
        *this->mController = this->mScreens.getGameController();
        // *this->mScreen = make_shared<GameScreen>(this->mGame);
        // *this->mController = make_shared<GameController<T>>(this->mScreen, this->mController, this->mGame);
        return true;
    }
};

#endif