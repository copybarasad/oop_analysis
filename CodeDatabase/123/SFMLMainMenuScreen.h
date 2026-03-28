#ifndef SFML_MAIN_MENU_SCREEN
#define SFML_MAIN_MENU_SCREEN

#include "SFMLScreen.h"
#include "MainMenuScreen.h"
#include "../HasFont.h"

class SFMLMainMenuScreen : public MainMenuScreen, public SFMLScreen
{
private:
    Text *mTitle;
    Text *mOptions;

public:
    SFMLMainMenuScreen();

    void render() override;

    ~SFMLMainMenuScreen();
};

#endif