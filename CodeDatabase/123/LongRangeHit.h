#ifndef HIT_BY_CLICK
#define HIT_BY_CLICK

#include "ICommand.h"
#include "../../GameSystem/Map.h"
#include "../../GameSystem/Player.h"

class LongRangeHit : public ICommand
{
private:
    shared_ptr<Player> mPlayer;
    Game *mGame;
    Vec2 *mHitPos;

public:
    LongRangeHit(shared_ptr<Player> player, Game *game, Vec2 *hitPos)
    {
        mPlayer = player;
        mGame = game;
        mHitPos = hitPos;
    }

    bool execute() override
    {

        if (!this->mGame->getPlayer()->isSlowed())
        {
            if (*mHitPos == Vec2(-1, -1))
                return false;
            shared_ptr<Entity> buff = mGame->getCell({mHitPos->x, mHitPos->y}).ent;

            if (mGame->getPlayer()->getHitType() == HitType::LongRangeCombat)
            {
                if (buff.get() == nullptr)
                {
                    return false;
                }
                buff->dealDamage(mGame->getPlayer()->getLongRangeCombatDamage());
                if (buff->isDead())
                {
                    mGame->getPlayer()->addExp(buff->getExpForKill());
                }
            }
            else
            {
                if (typeid(*(mGame->getPlayer()->getSpell(mGame->getPlayer()->getSelection()))) == typeid(DirectDamageSpell))
                {
                    if (buff.get() == nullptr)
                    {
                        return false;
                    }
                    mGame->getPlayer()->getSpell(mGame->getPlayer()->getSelection())->use(*(mGame->getMap()), mGame->getPlayer(), *mHitPos);
                }
                else
                {
                    mGame->getPlayer()->getSpell(mGame->getPlayer()->getSelection())->use(*(mGame->getMap()), mGame->getPlayer(), *mHitPos);
                }
            }
        }
        else
            this->mGame->getPlayer()->setSlowDown(false);

        return true;
    }
};

#endif