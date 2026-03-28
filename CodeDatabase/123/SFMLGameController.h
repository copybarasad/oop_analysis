#ifndef SFML_GAME_CONTROLLER
#define SFML_GAME_CONTROLLER

#include "../LogicControllers/IController.h"
#include "../Commands/MoveCommand.h"
#include "../Commands/ChooseHitType.h"
#include "../Commands/LongRangeHit.h"
#include "../Commands/SkipMove.h"
#include "../Commands/ChooseHandSelection.h"
#include "../Commands/GoToPause.h"
#include "../Commands/ExitFromProgram.h"
#include "../../Screens/IScreen.h"

#include "../../../Renderers/SingleWindow.h"
#include "../LogicControllers/GameController.h"
#include "../../../Screens/IScreen.h"

#define CELL_SPRITE_SIZE 32
#define CHARACTER_SPRITE_SIZE 48

class SFMLGameController : public GameController<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>
{
private:
    SingleWindow &mWindow = SingleWindow::getInstance();

public:
    using GameController::GameController;

    void handleEvent(std::variant<monostate, Keyboard::Key, Mouse::Button, bool> event) override
    {
        if (holds_alternative<Mouse::Button>(event))
        {
            mHitPos = getCellPos(Mouse::getPosition(*mWindow.getWindow()));
        }

        GameController<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>::handleEvent(event);
    }

    void setDefaultCommands() override
    {
        this->mConcreteMap = {
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::W, make_shared<MoveCommand>(mGame, mGame->getPlayer(), Vec2(0, 1))),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::A, make_shared<MoveCommand>(mGame, mGame->getPlayer(), Vec2(-1, 0))),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::S, make_shared<MoveCommand>(mGame, mGame->getPlayer(), Vec2(0, -1))),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::D, make_shared<MoveCommand>(mGame, mGame->getPlayer(), Vec2(1, 0))),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Space, make_shared<SkipMove>()),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Mouse::Button::Left, make_shared<LongRangeHit>(mGame->getPlayer(), mGame, &mHitPos)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num1, make_shared<ChooseHitType>(mGame, HitType::CloseCombat)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num2, make_shared<ChooseHitType>(mGame, HitType::LongRangeCombat)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Num3, make_shared<ChooseHitType>(mGame, HitType::Hand)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad1, make_shared<ChooseHandSelection>(mGame, 0)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad2, make_shared<ChooseHandSelection>(mGame, 1)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad3, make_shared<ChooseHandSelection>(mGame, 2)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad4, make_shared<ChooseHandSelection>(mGame, 3)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad5, make_shared<ChooseHandSelection>(mGame, 4)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad6, make_shared<ChooseHandSelection>(mGame, 5)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad7, make_shared<ChooseHandSelection>(mGame, 6)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad8, make_shared<ChooseHandSelection>(mGame, 7)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Numpad9, make_shared<ChooseHandSelection>(mGame, 8)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(sf::Keyboard::Key::Escape, make_shared<GoToPause<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>>>(mScreen, mController, this->mGame)),
            pair<std::variant<monostate, Keyboard::Key, Mouse::Button, bool>, shared_ptr<ICommand>>(true, make_shared<ExitFromSFMLProgram>())};
    }

private:
    Vector2i getCellPos(Vector2i pos)
    {
        Vector2f origin = Vector2f(mWindow.getWindow()->getSize().x / 2.f - mGame->getSize().x * CELL_SPRITE_SIZE / 2.f, 0.f);

        pos -= (Vector2i)origin;
        for (int y = 0; y < mGame->getSize().y; y++)
        {
            for (int x = 0; x < mGame->getSize().x; x++)
            {
                IntRect buff({CELL_SPRITE_SIZE * x, CELL_SPRITE_SIZE * y}, {CELL_SPRITE_SIZE, CELL_SPRITE_SIZE});
                if (buff.contains(pos))
                    return Vector2i({x, mGame->getSize().y - y - 1});
            }
        }

        return Vector2i(-1, -1);
    }
};

#endif