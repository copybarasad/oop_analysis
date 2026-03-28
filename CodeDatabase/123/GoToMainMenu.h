#ifndef GO_TO_MAIN_MENU
#define GO_TO_MAIN_MENU

#include "GoToScreen.h"
#include "../../Screens/MainMenuScreen.h"

template <typename T>
class GoToMainMenu : public GoToScreen<T>
{
public:
    using GoToScreen<T>::GoToScreen;

    bool execute() override
    {
        *this->mScreen = nullptr;
        *this->mController = nullptr;
        *this->mScreen = this->mScreens.getMainMenuScreen();
        *this->mController = this->mScreens.getMainMenuController();
        return true;
    }
};

#endif