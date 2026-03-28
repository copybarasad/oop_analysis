#include "SFMLTransitionToNewLevelScreen.h"
#include <random>

SFMLTransitionToNewLevelScreen::SFMLTransitionToNewLevelScreen(Game *game) : TransitionToNewLevelScreen(game)
{
}

void SFMLTransitionToNewLevelScreen::render()
{

    mTitle = new Text(mFont, format("{} --> {} \nCHOOSE UPGRADE", mGame->getLevel(), mGame->getLevel() + 1));
    mTitle->setCharacterSize(60);
    mTitle->setOrigin(Vector2f(0.5, 0.5));
    mTitle->setPosition(Vector2f(mWindow.getWindow()->getSize().x / 2.f, mWindow.getWindow()->getSize().y / 3.f));

    string optStr = format("Experience: {} \nCost: {} \n0. Resume without upgrade ", mGame->getPlayer().get()->getExp(), 40);
    if (mGame->getPlayer().get()->getExp() >= 40)
    {
        optStr += format("\n1. Update life {} --> {} \n2. Update melee damage {} --> {} \n3. Update long-range damage {} --> {}\n",
                         mGame->getPlayer().get()->getMaxLife(), (int)(mGame->getPlayer().get()->getMaxLife() * 1.2) + 1,
                         mGame->getPlayer().get()->getMeleeDamage(), (int)(mGame->getPlayer().get()->getMeleeDamage() * 1.2) + 1,
                         mGame->getPlayer().get()->getLongRangeCombatDamage(), (int)(mGame->getPlayer().get()->getLongRangeCombatDamage() * 1.2) + 1);

        for (size_t i = 0; i < mGame->getPlayer().get()->getHand()->getSize(); i++)
        {
            optStr += format("{}. {} (lvl {} --> {})\n", i + 5, mGame->getPlayer().get()->getHand()->getSpell(i)->getName(),
                             mGame->getPlayer().get()->getHand()->getSpell(i)->getLevel(),
                             mGame->getPlayer().get()->getHand()->getSpell(i)->getLevel() + 1);
        }
    }

    mOptions = new Text(mFont, optStr);
    mOptions->setCharacterSize(30);
    mOptions->setOrigin(Vector2f(0.5, 0.5));
    mOptions->setPosition(Vector2f(mWindow.getWindow()->getSize().x / 2.f, mWindow.getWindow()->getSize().y / 3.f + 130));

    mWindow.getWindow()->clear();
    mWindow.getWindow()->draw(*mTitle);
    mWindow.getWindow()->draw(*mOptions);
    mWindow.getWindow()->display();
    delete mTitle;
    delete mOptions;
}