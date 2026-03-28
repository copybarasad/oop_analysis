#include "SpellHandDeserializer.hpp"

SpellHand* SpellHandDeserializer::deserialize(const std::string& data) {
    auto keyValues = parseKeyValuePairs(data);

    validateRequiredFields(keyValues, {"level", "capacity", "spells"});
    
    size_t level = parseUnsignedInt(keyValues["level"], "hand level");
    size_t capacity = parseUnsignedInt(keyValues["capacity"], "hand capacity");
    
    if (capacity == 0) {
        throw ValidationException("hand capacity cannot be zero");
    }

    auto spells = split(keyValues["spells"], ';');
    
    SpellHand* hand;
    if (!spells.empty()) {
        hand = new SpellHand(capacity, spells[0], level);
    }
    else {
        hand = new SpellHand(capacity, "", level);
    }

    for (size_t i = 1; i < spells.size(); i++) {
        if (!spells[i].empty()) {
            if (spells[i] != "direct" && spells[i] != "area" && spells[i] != "buff" &&
                spells[i] != "trap" && spells[i] != "ally") {
                throw ValidationException("Cannot add spell to hand, invalid type");
            }
            if (!hand->addSpell(spells[i])) {
                throw ValidationException("Cannot add spell to hand, capacity exceeded");
            }
        }
    }
    
    return hand;
}