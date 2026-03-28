#ifndef MOVE_COMMAND
#define MOVE_COMMAND

#include "ICommand.h"
#include "../../GameSystem/Player.h"
#include "../../GameSystem/Game.h"

class MoveCommand : public ICommand
{
private:
    shared_ptr<Player> mPlayer;
    Game *mGame;
    Vec2 mNewPos;

public:
    MoveCommand(Game *game, shared_ptr<Player> player, Vec2 newPos)
    {
        mGame = game;
        mPlayer = player;
        mNewPos = newPos;
    }

    bool execute() override
    {
        if (!this->mGame->getPlayer()->isSlowed())
        {
            mGame->moveCharacterTo(mGame->getPlayer(), mGame->getPlayer()->getPosition() + mNewPos);
        }
        else
            this->mGame->getPlayer()->setSlowDown(false);

        return true;
    }
};

#endif