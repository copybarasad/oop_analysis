#ifndef UPDATE_NOTHING
#define UPDATE_NOTHING

#include "UpdatePlayer.h"

template <typename T>
class UpdateNothing : public UpdatePlayer<T>
{

public:
    using UpdatePlayer<T>::UpdatePlayer;

    bool execute() override
    {
        GoToGame<T> g(this->mScreen, this->mController, this->mGame);
        this->mGame->setNewLevel();
        return g.execute();
    }
};

#endif