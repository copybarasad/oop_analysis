#ifndef SFML_PAUSE_CONTROLLER
#define SFML_PAUSE_CONTROLLER

#include "../LogicControllers/IController.h"
#include <variant>
#include "../Commands/ExitFromProgram.h"
// #include "../../../Renderers/SingleWindow.h"
#include "../LogicControllers/PauseController.h"
#include "../../../Screens/IScreen.h"
#include "../Commands/GoToGame.h"
#include "../../Commands/GoToMainMenu.h"
#include "../../Commands/SaveGame.h"
#include "../../Commands/LoadGame.h"

#include <SFML/Graphics.hpp>
using namespace sf;

class SFMLPauseController : public PauseController<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>
{
public:
    using PauseController::PauseController;

    void setDefaultCommands() override
    {
        this->mConcreteMap = {
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num1, make_shared<GoToGame<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(mScreen, mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num2, make_shared<SaveGame>(this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num3, make_shared<LoadGame<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num4, make_shared<GoToMainMenu<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(mScreen, mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Escape, make_shared<GoToGame<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(mScreen, mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(true, make_shared<ExitFromSFMLProgram>())};
    }
};
#endif