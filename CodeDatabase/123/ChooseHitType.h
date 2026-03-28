#ifndef CHOOSE_HIT_TYPE
#define CHOOSE_HIT_TYPE

#include "ICommand.h"
#include "../../GameSystem/Player.h"

class ChooseHitType : public ICommand
{
private:
    shared_ptr<Player> mPlayer;
    Game *mGame;
    HitType mType;

public:
    ChooseHitType(Game *game, HitType type)
    {
        // mPlayer = player;
        mGame = game;
        mType = type;
    }

    bool execute() override
    {
        if (!this->mGame->getPlayer()->isSlowed())
        {
            mGame->getPlayer()->setHitType(mType);
        }
        else
            this->mGame->getPlayer()->setSlowDown(false);

        return true;
    }
};

#endif