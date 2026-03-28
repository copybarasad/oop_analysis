#ifndef GO_TO_PAUSE
#define GO_TO_PAUSE

#include "GoToScreen.h"
#include "../../Screens/PauseScreen.h"
// #include "../Controllers/LogicControllers/PauseController.h"
// #include "../../Screens/IScreen.h"

template <typename T>
class GoToPause : public GoToScreen<T>
{
public:
    using GoToScreen<T>::GoToScreen;

    bool execute() override
    {
        *this->mScreen = nullptr;
        *this->mScreen = this->mScreens.getPauseScreen();
        *this->mController = this->mScreens.getPauseController();
        // *mScreen = make_shared<PauseScreen>();
        return false;
    }
};

#endif