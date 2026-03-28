#pragma once

#include "i_spell_card.hpp"

class ITrap : public ISpellCard{
public:
    virtual ~ITrap() = default;

    virtual bool can_place(World* world, int x, int y) = 0;
};