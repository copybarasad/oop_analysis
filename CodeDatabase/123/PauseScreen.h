#ifndef PAUSE_SCREEN
#define PAUSE_SCREEN

#include "IScreen.h"
#include "../HasFont.h"
#include "../InputSystem/Controllers/LogicControllers/IController.h"

class PauseScreen : public IScreen, public HasFont
{
public:
    // void render();
    void tick() override {}
};

#endif