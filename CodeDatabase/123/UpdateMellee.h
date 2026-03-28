#ifndef UPDATE_MELLEE
#define UPDATE_MELLEE

#include "UpdatePlayer.h"

template <typename T>
class UpdateMellee : public UpdatePlayer<T>
{
public:
    using UpdatePlayer<T>::UpdatePlayer;

    bool execute() override
    {
        this->mGame->getPlayer().get()->setMeleeDamage(this->mGame->getPlayer().get()->getMeleeDamage() * 1.2 + 1);
        this->mGame->getPlayer().get()->subExp(UPGRADE_COST);
        this->mGame->setNewLevel();
        GoToGame<T> g(this->mScreen, this->mController, this->mGame);

        return g.execute();
    }
};

#endif