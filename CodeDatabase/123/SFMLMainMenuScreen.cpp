#include "SFMLMainMenuScreen.h"

SFMLMainMenuScreen::SFMLMainMenuScreen()
{
    mTitle = new Text(mFont, "MY GAME");
    mTitle->setCharacterSize(60);
    mTitle->setOrigin(Vector2f(0.5, 0.5));
    mTitle->setPosition(Vector2f(mWindow.getWindow()->getSize().x / 2.f, mWindow.getWindow()->getSize().y / 3.f));

    mOptions = new Text(mFont, "1. New game \n2. Load game \n3. Exit");
    mOptions->setCharacterSize(30);
    mOptions->setOrigin(Vector2f(0.5, 0.5));
    mOptions->setPosition(Vector2f(mWindow.getWindow()->getSize().x / 2.f, mWindow.getWindow()->getSize().y / 3.f + 60));
}

void SFMLMainMenuScreen::render()
{
    mWindow.getWindow()->clear();
    mWindow.getWindow()->draw(*mTitle);
    mWindow.getWindow()->draw(*mOptions);
    mWindow.getWindow()->display();
}

SFMLMainMenuScreen::~SFMLMainMenuScreen()
{
}