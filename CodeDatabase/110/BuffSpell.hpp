#ifndef BUFF_SPELL_H
#define BUFF_SPELL_H

#include "../BaseSpell/Spell.hpp"
#include "../../SpellHand/SpellHand.hpp"

class BuffSpell : public Spell {
    private:
    SpellHand& hand;
    size_t level;

    public:
    BuffSpell (SpellHand& hand, size_t level=0);

    CastResult cast () override;
    void upgradeSpell (size_t level) override;
};

#endif