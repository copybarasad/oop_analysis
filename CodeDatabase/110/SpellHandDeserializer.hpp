#ifndef SPELL_HAND_DESER_H
#define SPELL_HAND_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"
#include "modules/Spells/SpellHand/SpellHand.hpp"

class SpellHandDeserializer : public BaseDeserializer {
    public:
    SpellHand* deserialize(const std::string& data);
};

#endif