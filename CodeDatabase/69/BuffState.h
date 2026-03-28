#pragma once

struct BuffState 
{
    int damageRangeBonus = 0;
    int areaSizeBonus = 0;
    int trapDamageBonus = 0;
    int summonCountBonus = 0;

    BuffState& operator+=(BuffState const& o);
};