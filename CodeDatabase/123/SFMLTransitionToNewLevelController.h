#ifndef SFML_TRANSITION_TO_NEW_LEVEL_CONTROLLER
#define SFML_TRANSITION_TO_NEW_LEVEL_CONTROLLER

#include "../LogicControllers/TransitionToNewLevelController.h"
#include "../../../Screens/IScreen.h"
#include "../../Commands/GoToNewGame.h"

class SFMLTransitionToNewLevelController : public TransitionToNewLevelController<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>
{
public:
    using TransitionToNewLevelController::TransitionToNewLevelController;

    void setDefaultCommands() override
    {
        this->mConcreteMap = {
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num1, make_shared<UpdateLife<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num2, make_shared<UpdateMellee<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num3, make_shared<UpdateLongRange<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num4, make_shared<UpdateSpell<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame, 0)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num5, make_shared<UpdateSpell<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame, 1)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num6, make_shared<UpdateSpell<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame, 2)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num7, make_shared<UpdateSpell<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame, 3)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num8, make_shared<UpdateSpell<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame, 4)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num9, make_shared<UpdateSpell<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame, 5)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(Keyboard::Key::Num0, make_shared<UpdateNothing<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(this->mScreen, this->mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(true, make_shared<ExitFromSFMLProgram>())};
    }
};

#endif