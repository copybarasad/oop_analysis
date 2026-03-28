#ifndef BUY_SPELL_F_H
#define BUY_SPELL_F_H

#include "../BaseChangeableFields/ChangebaleFields.hpp"
#include "../../Spells/SpellStore/SpellStore.hpp"
#include "../../Spells/SpellHand/SpellHand.hpp"

class BuySpellFields : public ChangeableFields {
    private:
    SpellStore& store;
    SpellHand& hand;
    size_t index;

    public:
    BuySpellFields (SpellStore& store, SpellHand& hand, size_t index);

    SpellStore& getStore () const;
    SpellHand& getHand () const;
    size_t getIndex () const;
};

#endif