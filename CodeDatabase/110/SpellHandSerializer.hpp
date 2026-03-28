#ifndef SPELL_HAND_SER_H
#define SPELL_HAND_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../../../../../Spells/SpellHand/SpellHand.hpp"

class SpellHandSerializer : public Serializable {
    private:
    const SpellHand& hand;
    
    public:
    SpellHandSerializer (const SpellHand& hand);
    
    std::string serialize () const override;
};

#endif