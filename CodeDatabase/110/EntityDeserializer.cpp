#include "EntityDeserializer.hpp"

Entity* EntityDeserializer::deserialize(const std::string& data) {
    auto keyValues = parseKeyValuePairs(data);
    
    validateRequiredFields(keyValues, {"id", "maxHealth", "currentHealth", "damage", "skipNextTurn", "type"});
    
    size_t id = parseUnsignedInt(keyValues["id"], "id");
    unsigned int maxHealth = parseUnsignedInt(keyValues["maxHealth"], "maxHealth");
    unsigned int currentHealth = parseUnsignedInt(keyValues["currentHealth"], "currentHealth");
    unsigned int damage = parseUnsignedInt(keyValues["damage"], "damage");
    bool skipNextTurn = parseBool(keyValues["skipNextTurn"], "skipNextTurn");

    if (maxHealth == 0) throw ValidationException("maxHealth cannot be zero");
    if (currentHealth > maxHealth) {
        throw ValidationException("currentHealth cannot exceed maxHealth");
    }
    
    std::string type = keyValues["type"];
    Entity* entity = nullptr;
    
    if (type == "PLAYER") {
        PlayerDeserializer playerDeserializer;
        entity = playerDeserializer.deserialize(data, id, maxHealth, currentHealth, damage);
    }
    else if (type == "ENEMY") {
        entity = new Enemy(id, maxHealth, currentHealth, damage);
    }
    else if (type == "ALLY") {
        entity = new Ally(id, maxHealth, currentHealth, damage);
    }
    else if (type == "BUILDING") {
        entity = deserializeBuilding(data, id, maxHealth, currentHealth, damage);
    }
    else if (type == "TOWER") {
        entity = deserializeTower(data, id, maxHealth, currentHealth, damage);
    }
    else if (type == "TRAP") {
        entity = new Trap(id, maxHealth, currentHealth, damage);
    }
    else {
        throw ValidationException("Unknown entity type: " + type);
    }

    if (skipNextTurn) {
        entity->setSkip();
    }
    
    return entity;
}

Building* EntityDeserializer::deserializeBuilding(const std::string& data, size_t id, unsigned int maxHealth, unsigned int currentHealth, unsigned int damage) {
    auto keyValues = parseKeyValuePairs(data);
    
    validateRequiredFields(keyValues, {"spawnCoolDown", "currentCoolDown", "enemyMinHealth", "enemyMinDamage"});
    
    unsigned int spawnCoolDown = parseUnsignedInt(keyValues["spawnCoolDown"], "spawnCoolDown");
    unsigned int currentCoolDown = parseUnsignedInt(keyValues["currentCoolDown"], "currentCoolDown");
    unsigned int enemyMinHealth = parseUnsignedInt(keyValues["enemyMinHealth"], "enemyMinHealth");
    unsigned int enemyMinDamage = parseUnsignedInt(keyValues["enemyMinDamage"], "enemyMinDamage");
    
    if (spawnCoolDown == 0) {
        throw ValidationException("spawnCoolDown cannot be zero");
    }
    if (currentCoolDown > spawnCoolDown) {
        throw ValidationException("currentCoolDown cannot exceed spawnCoolDown");
    }
    
    Building* building = new Building(id, maxHealth, currentHealth, spawnCoolDown, currentCoolDown, damage, std::make_pair(enemyMinHealth, enemyMinDamage));
    return building;
}

Tower* EntityDeserializer::deserializeTower(const std::string& data, size_t id, unsigned int maxHealth, unsigned int currentHealth, unsigned int damage) {
    auto keyValues = parseKeyValuePairs(data);
        
    validateRequiredFields(keyValues, {"range", "spellCoolDown", "currentCoolDown"});
    
    unsigned int range = parseUnsignedInt(keyValues["range"], "range");
    unsigned int spellCoolDown = parseUnsignedInt(keyValues["spellCoolDown"], "spellCoolDown");
    unsigned int currentCoolDown = parseUnsignedInt(keyValues["currentCoolDown"], "currentCoolDown");
    
    if (range == 0) {
        throw ValidationException("range cannot be zero");
    }
    if (spellCoolDown == 0) {
        throw ValidationException("spellCoolDown cannot be zero");
    }
    if (currentCoolDown > spellCoolDown) {
        throw ValidationException("currentCoolDown cannot exceed spellCoolDown");
    }
    
    Tower* tower = new Tower(id, maxHealth, currentHealth, spellCoolDown, currentCoolDown, range, damage);
    return tower;
}