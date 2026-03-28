#ifndef RANGE_FIELDS_H
#define RANGE_FIELDS_H

#include "../CastChangeableFields/CastChangeableFields.hpp"
#include "../../../Field/Field/Field.hpp"

class RangeFields : public CastFields {
    private:
    Field& field;
    std::pair<size_t, size_t> caster;
    std::pair<size_t, size_t> target;

    public:
    RangeFields (std::string spellName, Field& field, SpellHand& hand, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target);

    Field& getField () const;
    SpellHand& getHand () const;
    std::pair<size_t, size_t> getCaster () const;
    std::pair<size_t, size_t> getTarget () const;
};

#endif