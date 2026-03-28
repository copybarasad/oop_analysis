#ifndef UPDATE_SPELL
#define UPDATE_SPELL

#include "UpdatePlayer.h"

template <typename T>
class UpdateSpell : public UpdatePlayer<T>
{
private:
    int mSpellSelection;

public:
    UpdateSpell(shared_ptr<IScreen> *screen, shared_ptr<IController<T>> *controller, Game *game, int spell) : UpdatePlayer<T>(screen, controller, game)
    {
        mSpellSelection = spell;
    }

    bool execute() override
    {
        this->mGame->getPlayer().get()->getHand()->getSpell(this->mSpellSelection - 1)->update();
        this->mGame->getPlayer().get()->subExp(UPGRADE_COST);
        this->mGame->setNewLevel();
        GoToGame<T> g(this->mScreen, this->mController, this->mGame);

        return g.execute();
    }
};

#endif