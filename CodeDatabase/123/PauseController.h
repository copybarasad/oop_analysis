#ifndef PAUSE_CONTROLLER
#define PAUSE_CONTROLLER

#include "IController.h"
// #include "../../GameSystem/Game.h"
#include <variant>
#include "../Commands/ExitFromProgram.h"
#include "../../../Renderers/SingleWindow.h"
#include "../../../GameSystem/Game.h"
#include "../../Commands/GoToGame.h"
#include "../../Commands/GoToMainMenu.h"
#include "../../Commands/SaveGame.h"
#include "../../Commands/LoadGame.h"

template <typename T>
class PauseController : public IController<T>
{
protected:
public:
    PauseController(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game) : IController<T>(screen, controller, game)
    {
        this->mCommandMap =
            {
                pair<string, shared_ptr<ICommand>>("Resume button", make_shared<GoToGame<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Main menu save", make_shared<SaveGame>(this->mGame)),
                pair<string, shared_ptr<ICommand>>("Main menu load", make_shared<LoadGame<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Exit to Main menu", make_shared<GoToMainMenu<T>>(this->mScreen, this->mController, this->mGame))};
        // pair<string, shared_ptr<ICommand>>("Resume button", make_shared<GoToGame<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(mScreen, mController))};
        // pastring_view, new ChooseHitType(mPlayer, HitType::LongRangeCombat)),
        // pastring_view, new ChooseHitType(mPlayer, HitType::Hand)),
        // pair<string_view, ICommand *>(sf::Keyboard::Key::Escape, new GoToGame(mScreen)),
        //     pair<string_view, ICommand *>(true, new ExitFromSFMLProgram(mWindow))
    };
};

#endif