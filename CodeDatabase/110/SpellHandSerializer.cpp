#include "SpellHandSerializer.hpp"

SpellHandSerializer::SpellHandSerializer (const SpellHand& hand) :
    hand(hand) {}

std::string SpellHandSerializer::serialize () const {
    std::ostringstream oss;
    oss << "level:" << hand.getHandLevel() << ",";
    oss << "capacity:" << hand.getHandCapacity() << ",";
    oss << "spells:";
    
    for (size_t i = 0; i < hand.getSpellNumber(); ++i) {
        if (i > 0) oss << ";";
        oss << hand.getSpellName(i);
    }
    
    return oss.str();
}