#include "BuySpellFields.hpp"

BuySpellFields::BuySpellFields (SpellStore& store, SpellHand& hand, size_t index) :
    store(store), hand(hand), index(index) {}

SpellStore& BuySpellFields::getStore () const { return store; }

SpellHand& BuySpellFields::getHand () const { return hand; }

size_t BuySpellFields::getIndex () const { return index; }