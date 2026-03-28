#ifndef MAIN_MENU_CONTROLLER
#define MAIN_MENU_CONTROLLER

#include "IController.h"
#include "../../Commands/GoToNewGame.h"
#include "../../Commands/ExitFromProgram.h"
#include "../../Commands/SaveGame.h"
#include "../../Commands/LoadGame.h"

template <typename T>
class MainMenuController : public IController<T>
{
public:
    MainMenuController(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game) : IController<T>(screen, controller, game)
    {
        this->mCommandMap =
            {
                pair<string, shared_ptr<ICommand>>("Start new game", make_shared<GoToNewGame<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Main menu load", make_shared<LoadGame<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Exit from game", make_shared<ExitFromSFMLProgram>())};
    }
};

#endif