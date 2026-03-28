#include "RangeFields.hpp"

RangeFields::RangeFields (std::string spellName, Field& field, SpellHand& hand, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    CastFields(spellName, hand), field(field), caster(caster), target(target) {}

Field& RangeFields::getField () const { return field; }

SpellHand& RangeFields::getHand () const { return hand; }

std::pair<size_t, size_t> RangeFields::getCaster () const { return caster; }

std::pair<size_t, size_t> RangeFields::getTarget () const { return target; }