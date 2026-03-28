#ifndef IBATTLECARD_H
#define IBATTLECARD_H

#include "ispellcard.h"
#include "playerspellconfig.h"
#include "enemyspellconfig.h"

class IBattleCard : public ISpellCard
{
public:
    virtual ~IBattleCard() = default;
    virtual void apply(PlayerSpellConfig& config) = 0;
    virtual void apply(EnemySpellConfig& config) = 0;
};

#endif // IBATTLECARD_H
