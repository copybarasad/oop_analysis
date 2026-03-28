#ifndef ISCREEN
#define ISCREEN

#include <SFML/Graphics.hpp>
using namespace sf;
// #include "../InputSystem/Controllers/LogicControllers/IController.h"

// class Program;

class IScreen
{
protected:
    // IController<SFMLInput> *mController;

public:
    virtual void handleInput()
    {
        // mController->handleInput(true);
    }

    virtual void render() = 0;

    virtual void tick() = 0;

    // void setController(IController *controller)
    // {
    //     mController = controller;
    // }
};

#endif