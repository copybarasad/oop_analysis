#include "CastChangeableFields.hpp"

CastFields::CastFields (std::string spellName, SpellHand& hand) :
    spellName(spellName), hand(hand) {}

const std::string CastFields::getSpellName () const { return spellName; }

SpellHand& CastFields::getHand () const { return hand; }