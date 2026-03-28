#include "BuffState.h"

BuffState& BuffState::operator+=(BuffState const& o) 
{
    damageRangeBonus += o.damageRangeBonus;
    areaSizeBonus    += o.areaSizeBonus;
    trapDamageBonus  += o.trapDamageBonus;
    summonCountBonus += o.summonCountBonus;

    return *this;
}