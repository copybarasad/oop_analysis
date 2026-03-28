#ifndef SPELL_STORE_DESER_H
#define SPELL_STORE_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"
#include "modules/Spells/SpellStore/SpellStore.hpp"

class SpellStoreDeserializer : public BaseDeserializer {
    public:
    SpellStore* deserialize(const std::string& data);
};

#endif