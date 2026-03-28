#pragma once

#include "i_spell_card.hpp"

class IAreaDamage : public ISpellCard{
public:
    virtual ~IAreaDamage() = default;

    virtual int get_damage(Entity& entity) = 0;
};