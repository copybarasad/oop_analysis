#ifndef CAST_FIELDS_H
#define CAST_FIELDS_H

#include "../../BaseChangeableFields/ChangebaleFields.hpp"
#include "../../../Spells/SpellHand/SpellHand.hpp"

class CastFields : public ChangeableFields {
    protected:
    std::string spellName;
    SpellHand& hand;

    public:
    CastFields (std::string spellName, SpellHand& hand);

    const std::string getSpellName () const;
    SpellHand& getHand () const;
};

#endif