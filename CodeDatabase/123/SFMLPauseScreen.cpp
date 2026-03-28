#include "SFMLPauseScreen.h"

SFMLPauseScreen::SFMLPauseScreen(/*shared_ptr<IScreen> *screen*/) : SFMLScreen(), PauseScreen()
{
    // mController = new SFMLPauseController(window, screen);

    mTitle = new Text(mFont, "Pause");
    mTitle->setCharacterSize(60);
    mTitle->setOrigin(Vector2f(0.5, 0.5));
    mTitle->setPosition(Vector2f(mWindow.getWindow()->getSize().x / 2.f, mWindow.getWindow()->getSize().y / 3.f));

    mOptions = new Text(mFont, "1. Resume game \n2. Save game \n3. Load game \n4. Exit to main menu");
    mOptions->setCharacterSize(30);
    mOptions->setOrigin(Vector2f(0.5, 0.5));
    mOptions->setPosition(Vector2f(mWindow.getWindow()->getSize().x / 2.f, mWindow.getWindow()->getSize().y / 3.f + 60));
}

void SFMLPauseScreen::render()
{
    mWindow.getWindow()->clear();
    mWindow.getWindow()->draw(*mTitle);
    mWindow.getWindow()->draw(*mOptions);
    mWindow.getWindow()->display();
}

SFMLPauseScreen::~SFMLPauseScreen()
{
    delete mTitle;
    delete mOptions;
}
