#include "SpellStoreDeserializer.hpp"

SpellStore* SpellStoreDeserializer::deserialize(const std::string& data) {
    auto keyValues = parseKeyValuePairs(data);
    
    validateRequiredFields(keyValues, {"available_spells"});
    auto spells = split(keyValues["available_spells"], ';');
    for (auto& spell : spells) {
        if (spell != "direct" && spell != "area" && spell != "buff" &&
            spell != "trap" && spell != "ally") {
            throw ValidationException("Cannot add spell to hand, invalid type");
        }
    }
    
    SpellStore* store = new SpellStore(spells);
    
    return store;
}