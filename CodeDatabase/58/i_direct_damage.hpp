#pragma once

#include "i_spell_card.hpp"

class IDirectDamage : public ISpellCard{
public:
    virtual ~IDirectDamage() = default;

    virtual int get_damage(Entity& entity) = 0;
};