#include "GameScreen.h"
#include "../Program.h"
#include "PauseScreen.h"
#include "MainMenuScreen.h"
#include "TransitionToNewLevelScreen.h"

void GameScreen::tick()
{
    // if (mIsMoveDone)
    //     mGame->enemyMove();
    // if (mGame->getPlayer()->getExp() >= 40 && mGame->getPlayer()->getSize() < SPELL_CAPASITY)
    // {
    //     mGame->getPlayer()->addSpell();
    //     mGame->getPlayer()->subExp(40);
    // }
}

void GameScreen::handleInput()
{
    // if (mGame->getPlayer()->isSlowed())
    // {
    //     // mIsMoveDone = mController->handleInput(false);
    //     mGame->getPlayer()->setSlowDown(false);
    // }
    // else
    // {
    //     // mIsMoveDone = mController->handleInput(true);
    // }
}

// void GameScreen::render()
// {
//     mGame->setPosition({window.getSize().x / 2.f - mGame->getSize().x / 2.f, 0.f});
//     bool isMoveMade = false;

//     window.clear();
//     mGame->render(window);
//     window.display();
// }