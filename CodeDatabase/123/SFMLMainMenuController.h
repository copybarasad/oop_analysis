#ifndef SFML_MAIN_MENU_CONTROLLER
#define SFML_MAIN_MENU_CONTROLLER

#include "../LogicControllers/MainMenuController.h"
#include "../../../Screens/IScreen.h"
#include "../../Commands/GoToNewGame.h"

class SFMLMainMenuController : public MainMenuController<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>
{
public:
    using MainMenuController::MainMenuController;

    void setDefaultCommands() override
    {
        this->mConcreteMap = {
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num1, make_shared<GoToNewGame<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(mScreen, mController, mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num2, make_shared<LoadGame<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num3, make_shared<ExitFromSFMLProgram>())};
    }
};

#endif