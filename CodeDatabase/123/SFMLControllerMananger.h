#ifndef SFML_CONTROLLER
#define SFML_CONTROLLER

#include <SFML/Graphics.hpp>
using namespace sf;
#include <variant>
#include <memory>
using namespace std;
using SFMLInput = std::variant<monostate, Keyboard::Key, Mouse::Button, bool>;
#include "SFMLController/SFMLGameController.h"
#include "SFMLController/SFMLPauseController.h"
#include "SFMLController/SFMLMainMenuController.h"
#include "SFMLController/SFMLTransitionToNewLevelController.h"
#include "../../Renderers/SingleWindow.h"
#include "../../Screens/Screens.h"

class SFMLControllerMananger
{
protected:
    SingleWindow &mWindow;
    shared_ptr<SFMLGameController> mGameController;
    shared_ptr<SFMLPauseController> mPauseController;
    shared_ptr<SFMLMainMenuController> mMainMenuController;
    shared_ptr<SFMLTransitionToNewLevelController> mTransitionToNewLevelController;
    shared_ptr<IController<SFMLInput>> mCurrent;
    Screens<SFMLInput> mScreens;

public:
    SFMLControllerMananger(shared_ptr<IScreen> *screen, Game *game) : mWindow(SingleWindow::getInstance())
    {
        mGameController = make_shared<SFMLGameController>(screen, &mCurrent, game);
        mPauseController = make_shared<SFMLPauseController>(screen, &mCurrent, game);
        mMainMenuController = make_shared<SFMLMainMenuController>(screen, &mCurrent, game);
        mTransitionToNewLevelController = make_shared<SFMLTransitionToNewLevelController>(screen, &mCurrent, game);
        mGameController->setDefaultCommands();
        mPauseController->setDefaultCommands();
        mMainMenuController->setDefaultCommands();
        mTransitionToNewLevelController->setDefaultCommands();
        mCurrent = mMainMenuController;
        mScreens.setGameController(mGameController);
        mScreens.setPauseController(mPauseController);
        mScreens.setMainMenuController(mMainMenuController);
        mScreens.setTransitionToNewLevelController(mTransitionToNewLevelController);
    }

    void handleInput() // override
    {
        SFMLInput ans;
        while (const std::optional event = mWindow.getWindow()->pollEvent())
        {
            // if (isMove)
            // {
            if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                ans = keyReleased->code;
                // return keyReleased->code;
            }
            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>())
            {
                ans = mouseButtonPressed->button;
                // return mouseButtonPressed->button;
            }
            if (event->is<sf::Event::Closed>())
            {
                ans = true;
                // return true;
            }
            // }
        }
        if (holds_alternative<monostate>(ans))
        {
            return;
        }
        mCurrent->handleEvent(ans);
    }
};

#endif