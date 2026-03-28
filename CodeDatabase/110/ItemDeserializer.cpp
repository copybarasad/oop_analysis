#include "ItemDeserializer.hpp"

Item* ItemDeserializer::deserialize(const std::string& data) {
    auto parts = split(data, ',');
    if (parts.size() < 2) {
        throw ValidationException("Invalid item data: " + data);
    }
    
    if (parts[0] != "ITEM") {
        throw ValidationException("Invalid item format: " + data);
    }
    
    std::string name = parts[1];
    Item* item = nullptr;
    
    if (parts.size() > 2 && parts[2] == "WEAPON") {
        item = deserializeWeapon(name, parts);
    }
    else if (parts.size() > 2 && parts[2] == "ARMOR") {
        item = deserializeArmor(name, parts);
    }
    else {
        item = new Item(name);
    }
    
    return item;
}

Weapon* ItemDeserializer::deserializeWeapon(const std::string& name, const std::vector<std::string>& parts) {
    auto keyValues = parseKeyValuePairs(combineParts(parts, 3));
        
        validateRequiredFields(keyValues, {"type", "minDamage", "maxDamage", "critChance", "critBonus", "range"});
        
        CombatMode type = parseCombatMode(keyValues["type"]);
        int minDamage = parseInt(keyValues["minDamage"], "minDamage");
        int maxDamage = parseInt(keyValues["maxDamage"], "maxDamage");
        float critChance = parseFloat(keyValues["critChance"], "critChance");
        float critBonus = parseFloat(keyValues["critBonus"], "critBonus");
        int range = parseInt(keyValues["range"], "range");

        if (minDamage < 0) throw ValidationException("minDamage cannot be negative");
        if (maxDamage < minDamage) throw ValidationException("maxDamage cannot be less than minDamage");
        if (critChance < 0 || critChance > 1) throw ValidationException("critChance must be between 0 and 1");
        if (critBonus < 1) throw ValidationException("critBonus cannot be less than 1");
        if (range < 0) throw ValidationException("range cannot be negative");
        
        return new Weapon(name, type, minDamage, maxDamage, critChance, critBonus, range);
}

Armor* ItemDeserializer::deserializeArmor(const std::string& name, const std::vector<std::string>& parts) {
    auto keyValues = parseKeyValuePairs(combineParts(parts, 3));
    
    validateRequiredFields(keyValues, {"armorClass"});
    
    int armorClass = parseInt(keyValues["armorClass"], "armorClass");
    
    if (armorClass < 0) {
        throw ValidationException("armorClass cannot be negative");
    }
    
    return new Armor(name, armorClass);
}

std::string ItemDeserializer::combineParts(const std::vector<std::string>& parts, size_t startIndex) const {
    std::string result;
    for (size_t i = startIndex; i < parts.size(); ++i) {
        if (i > startIndex) {
            result += ",";
        }
        result += parts[i];
    }
    
    return result;
}