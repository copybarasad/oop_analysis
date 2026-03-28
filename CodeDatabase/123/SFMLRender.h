#ifndef GAME_WITH_SFML_RENDER
#define GAME_WITH_SFML_RENDER

#include "IRender.h"
#include <SFML/Graphics.hpp>
using namespace sf;
#include "SingleWindow.h"

class SFMLRender : public IRender
{
private:
    SingleWindow &mWindow;
    Screens<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>> mScreens;

public:
    SFMLRender();

    bool isEnd() override;

    void start() override;
};

#endif