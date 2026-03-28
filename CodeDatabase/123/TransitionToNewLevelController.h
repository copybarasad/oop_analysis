#ifndef TRANSITION_TO_NEW_LEVEL_CONTROLLER
#define TRANSITION_TO_NEW_LEVEL_CONTROLLER

#include "IController.h"
#include "../../Commands/GoToGame.h"
#include "../../Commands/ExitFromProgram.h"
#include "../../Commands/UpdateLife.h"
#include "../../Commands/UpdateMellee.h"
#include "../../Commands/UpdateLongRange.h"
#include "../../Commands/UpdateSpell.h"
#include"../../Commands/UpdateNothing.h"

template <typename T>
class TransitionToNewLevelController : public IController<T>
{
public:
    TransitionToNewLevelController(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game) : IController<T>(screen, controller, game)
    {
        this->mCommandMap =
            {
                pair<string, shared_ptr<ICommand>>("Update life", make_shared<UpdateLife<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Update Mellee damage", make_shared<UpdateMellee<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Update long-range damage", make_shared<UpdateLongRange<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Update spell 1", make_shared<UpdateSpell<T>>(this->mScreen, this->mController, this->mGame, 0)),
                pair<string, shared_ptr<ICommand>>("Update spell 2", make_shared<UpdateSpell<T>>(this->mScreen, this->mController, this->mGame, 1)),
                pair<string, shared_ptr<ICommand>>("Update spell 3", make_shared<UpdateSpell<T>>(this->mScreen, this->mController, this->mGame, 2)),
                pair<string, shared_ptr<ICommand>>("Update spell 4", make_shared<UpdateSpell<T>>(this->mScreen, this->mController, this->mGame, 3)),
                pair<string, shared_ptr<ICommand>>("Update spell 5", make_shared<UpdateSpell<T>>(this->mScreen, this->mController, this->mGame, 4)),
                pair<string, shared_ptr<ICommand>>("Update spell 6", make_shared<UpdateSpell<T>>(this->mScreen, this->mController, this->mGame, 5)),
                pair<string, shared_ptr<ICommand>>("Skip update", make_shared<UpdateNothing<T>>(this->mScreen, this->mController, this->mGame)),
                pair<string, shared_ptr<ICommand>>("Exit from game", make_shared<ExitFromProgram<T>>())};
    }
};

#endif