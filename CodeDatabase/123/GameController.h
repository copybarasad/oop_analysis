#ifndef GAME_CONTROLLER
#define GAME_CONTROLLER

#include "IController.h"
#include "../../Commands/MoveCommand.h"
#include "../../Commands/ChooseHitType.h"
#include "../../Commands/LongRangeHit.h"
#include "../../Commands/SkipMove.h"
#include "../../Commands/ChooseHandSelection.h"
#include "../../Commands/GoToScreen.h"
#include "../../Commands/ExitFromProgram.h"
#include "../../Commands/GoToTransitionToNewLevel.h"
#include "../../Commands/GoToMainMenu.h"
#include "../../Screens/IScreen.h"
#include <variant>
// #include "../../../GameSystem/Game.h"

template <typename T>
class GameController : public IController<T>
{
protected:
    Vec2 mHitPos;

public:
    // GameController() = default;

    virtual void handleEvent(T event) override
    {
        bool isMoveDone = false;
        if (this->mConcreteMap.contains(event))
        {
            isMoveDone = this->mConcreteMap[event]->execute();
        }

        if (isMoveDone)
        {
            this->mGame->enemyMove();
        }
        if (this->mGame->getPlayer()->getExp() >= 40 && this->mGame->getPlayer()->getSize() < SPELL_CAPASITY)
        {
            this->mGame->getPlayer()->addSpell();
            this->mGame->getPlayer()->subExp(40);
        }
        
        if (this->mGame->gameOver())
        {

            if (this->mGame->isWin())
            {
                GoToTransitionToNewLevel<T> g(this->mScreen, this->mController, this->mGame);
                g.execute();

                return;
            }
            else
            {
                GoToMainMenu<T> g(this->mScreen, this->mController, this->mGame);
                g.execute();
                return;
            }
        }
        // return isMoveDone;
    }

    GameController(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game) : IController<T>(screen, controller, game)
    {
        this->mCommandMap = {
            pair<string, shared_ptr<ICommand>>("Move North", make_shared<MoveCommand>(this->mGame, this->mGame->getPlayer(), Vec2(0, 1))),
            pair<string, shared_ptr<ICommand>>("Move West", make_shared<MoveCommand>(this->mGame, this->mGame->getPlayer(), Vec2(-1, 0))),
            pair<string, shared_ptr<ICommand>>("Move South", make_shared<MoveCommand>(this->mGame, this->mGame->getPlayer(), Vec2(0, -1))),
            pair<string, shared_ptr<ICommand>>("Move East", make_shared<MoveCommand>(this->mGame, this->mGame->getPlayer(), Vec2(1, 0))),
            pair<string, shared_ptr<ICommand>>("Skip move", make_shared<SkipMove>()),
            pair<string, shared_ptr<ICommand>>("Make long-range hit", make_shared<LongRangeHit>(this->mGame->getPlayer(), this->mGame, &mHitPos)),
            pair<string, shared_ptr<ICommand>>("Switch to close combat", make_shared<ChooseHitType>(this->mGame, HitType::CloseCombat)),
            pair<string, shared_ptr<ICommand>>("Switch to long-range combat", make_shared<ChooseHitType>(this->mGame, HitType::LongRangeCombat)),
            pair<string, shared_ptr<ICommand>>("Switch to long-range hand", make_shared<ChooseHitType>(this->mGame, HitType::Hand)),
            pair<string, shared_ptr<ICommand>>("Choose spell 1", make_shared<ChooseHandSelection>(this->mGame, 0)),
            pair<string, shared_ptr<ICommand>>("Choose spell 2", make_shared<ChooseHandSelection>(this->mGame, 1)),
            pair<string, shared_ptr<ICommand>>("Choose spell 3", make_shared<ChooseHandSelection>(this->mGame, 2)),
            pair<string, shared_ptr<ICommand>>("Choose spell 4", make_shared<ChooseHandSelection>(this->mGame, 3)),
            pair<string, shared_ptr<ICommand>>("Choose spell 5", make_shared<ChooseHandSelection>(this->mGame, 4)),
            pair<string, shared_ptr<ICommand>>("Choose spell 6", make_shared<ChooseHandSelection>(this->mGame, 5)),
            pair<string, shared_ptr<ICommand>>("Choose spell 7", make_shared<ChooseHandSelection>(this->mGame, 6)),
            pair<string, shared_ptr<ICommand>>("Choose spell 8", make_shared<ChooseHandSelection>(this->mGame, 7)),
            pair<string, shared_ptr<ICommand>>("Choose spell 9", make_shared<ChooseHandSelection>(this->mGame, 8)),
            pair<string, shared_ptr<ICommand>>("Go to pause", make_shared<GoToPause<T>>(this->mScreen, this->mController, this->mGame))};
        // pair<string, ICommand *>(true, new ExitFromSFMLProgram(win))};
    }
};

#endif