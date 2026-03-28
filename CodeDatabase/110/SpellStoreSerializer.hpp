#ifndef SPELL_STORE_SER_H
#define SPELL_STORE_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../../../../../Spells/SpellStore/SpellStore.hpp"

class SpellStoreSerializer : public Serializable {
    private:
    const SpellStore& store;
    
    public:
    SpellStoreSerializer (const SpellStore& store);
    
    std::string serialize () const override;
};

#endif