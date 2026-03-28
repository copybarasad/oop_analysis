#ifndef MAIN_MENU_SCREEN
#define MAIN_MENU_SCREEN

#include "IScreen.h"
#include "../HasFont.h"
#include "../GameSystem/Game.h"
#include "../InputSystem/Controllers/LogicControllers/IController.h"

class MainMenuScreen : public IScreen, public HasFont
{
protected:
    // Game *mGame;

public:
    MainMenuScreen(/*, shared_ptr<IScreen> *current*/)
    {
        // mGame = game;
        // mCurrentScreen = current;
    }
    void tick() override {}
};

#endif