#ifndef SFML_SREEN
#define SFML_SREEN

#include "IScreen.h"
#include <SFML/Graphics.hpp>
using namespace sf;
#include "../Renderers/SingleWindow.h"
using namespace std;

class SFMLScreen
{
protected:
    SingleWindow &mWindow;

public:
    SFMLScreen() : mWindow(SingleWindow::getInstance())
    {
        // mWindow = win;
    }
};

#endif