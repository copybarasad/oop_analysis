#ifndef UPDATE_LIFE
#define UPDATE_LIFE

#include "UpdatePlayer.h"

template <typename T>
class UpdateLife : public UpdatePlayer<T>
{
public:
    using UpdatePlayer<T>::UpdatePlayer;

    bool execute() override
    {
        this->mGame->getPlayer().get()->setMaxLife(this->mGame->getPlayer().get()->getMaxLife() * 1.2 + 1);
        this->mGame->getPlayer().get()->subExp(UPGRADE_COST);
        this->mGame->setNewLevel();
        GoToGame<T> g(this->mScreen, this->mController, this->mGame);

        return g.execute();
    }
};

#endif