#include "SpellStoreSerializer.hpp"

SpellStoreSerializer::SpellStoreSerializer (const SpellStore& store) :
    store(store) {}

std::string SpellStoreSerializer::serialize () const {
    std::ostringstream oss;
    oss << "available_spells:";
    
    for (size_t i = 0; i < store.getSpellNumber(); ++i) {
        if (i > 0) oss << ";";
        oss << store.getSpell(i);
    }
    
    return oss.str();
}