#include "SFMLRender.h"
#include "../Screens/SFMLGameScreen.h"
#include "../Screens/SFMLPauseScreen.h"
#include "../Screens/SFMLMainMenuScreen.h"
#include "../Screens/SFMLTransitionToNewLevelScreen.h"

SFMLRender::SFMLRender() : IRender(), mWindow(SingleWindow::getInstance())
{
}

bool SFMLRender::isEnd()
{
    return !mWindow.getWindow()->isOpen();
}

void SFMLRender::start()
{
    mScreens.setGameScreen(make_shared<SFMLGameScreen>(mGame.get()));
    mScreens.setPauseScreen(make_shared<SFMLPauseScreen>());
    mScreens.setMainMenuScreen(make_shared<SFMLMainMenuScreen>());
    mScreens.setTransitionToNewLevelScreen(make_shared<SFMLTransitionToNewLevelScreen>(mGame.get()));

    mCurrentScreen = mScreens.getMainMenuScreen();
}
