#ifndef BUFF_FIELDS_H
#define BUFF_FIELDS_H

#include "../CastChangeableFields/CastChangeableFields.hpp"

class BuffFields : public CastFields {
    public:
    BuffFields (std::string spellName, SpellHand& hand);
};

#endif