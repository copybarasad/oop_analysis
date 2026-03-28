#include "PlayerDeserializer.hpp"

Player* PlayerDeserializer::deserialize(const std::string& data, size_t id, unsigned int maxHealth, unsigned int currentHealth, unsigned int baseDamage) {
    auto keyValues = parseKeyValuePairs(data);

    validateRequiredFields(keyValues, {"points", "mode", "weapon", "armor"});
    
    unsigned int points = parseUnsignedInt(keyValues["points"], "points");
    CombatMode mode = parseCombatMode(keyValues["mode"]);

    Weapon* weapon = nullptr;
    if (keyValues["weapon"] != "NONE") {
        ItemDeserializer itemDeserializer;
        Item* item = itemDeserializer.deserialize(keyValues["weapon"]);
        weapon = dynamic_cast<Weapon*>(item);
        if (!weapon) {
            delete item;
            throw ValidationException("Expected weapon but got different item type");
        }
    }

    Armor* armor = nullptr;
    if (keyValues["armor"] != "NONE") {
        ItemDeserializer itemDeserializer;
        Item* item = itemDeserializer.deserialize(keyValues["armor"]);
        armor = dynamic_cast<Armor*>(item);
        if (!armor) {
            delete item;
            throw ValidationException("Expected armor but got different item type");
        }
    }

    std::vector<Item*> inventory;
    auto inventoryItems = split(keyValues["inventory"], ';');
    for (const auto& itemData : inventoryItems) {
        if (!itemData.empty()) {
            ItemDeserializer itemDeserializer;
            Item* item = itemDeserializer.deserialize(itemData);
            inventory.push_back(item);
        }
    }

    Player* player = new Player(id, maxHealth, currentHealth, baseDamage, points, inventory, weapon, armor, mode);
    
    return player;
}