#ifndef UPDATE_LONG_RANGE
#define UPDATE_LONG_RANGE

#include "UpdatePlayer.h"

template <typename T>
class UpdateLongRange : public UpdatePlayer<T>
{
public:
    using UpdatePlayer<T>::UpdatePlayer;

    bool execute() override
    {
        this->mGame->getPlayer().get()->setLongRangeCombatDamage(this->mGame->getPlayer().get()->getLongRangeCombatDamage() * 1.2 + 1);
        this->mGame->getPlayer().get()->subExp(UPGRADE_COST);
        this->mGame->setNewLevel();
        GoToGame<T> g(this->mScreen, this->mController, this->mGame);

        return g.execute();
    }
};

#endif