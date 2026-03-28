#include "EntitySerializer.hpp"

EntitySerializer::EntitySerializer (const Entity& entity) :
    entity(entity) {}

std::string EntitySerializer::serialize() const {
    std::ostringstream oss;
    oss << "id:" << entity.getID() << ",";
    oss << "maxHealth:" << entity.getMaxHealth() << ",";
    oss << "currentHealth:" << entity.getCurrentHealth() << ",";
    oss << "damage:" << entity.calculateDamage() << ",";
    oss << "skipNextTurn:" << (entity.isSkipping() ? "1" : "0") << ",";
    
    if (dynamic_cast<const Player*>(&entity)) {
        PlayerSerializer playerSerializer(*dynamic_cast<const Player*>(&entity));
        oss << playerSerializer.serialize();
    }
    else if (dynamic_cast<const Enemy*>(&entity)) {
        oss << "type:ENEMY";
    }
    else if (dynamic_cast<const Ally*>(&entity)) {
        oss << "type:ALLY";
    }
    else if (dynamic_cast<const Building*>(&entity)) {
        const Building& building = *dynamic_cast<const Building*>(&entity);
        oss << "type:BUILDING,";
        oss << "spawnCoolDown:" << building.getMaxCoolDown() << ",";
        oss << "currentCoolDown:" << building.getSpawnCoolDown() << ",";
        auto stats = building.getEnemyMinStats();
        oss << "enemyMinHealth:" << stats.first << ",";
        oss << "enemyMinDamage:" << stats.second;
    }
    else if (dynamic_cast<const Tower*>(&entity)) {
        const Tower& tower = *dynamic_cast<const Tower*>(&entity);
        oss << "type:TOWER,";
        oss << "range:" << tower.getRange() << ",";
        oss << "spellCoolDown:" << tower.getSpellCoolDown() << ",";
        oss << "currentCoolDown:" << tower.getCurrentCoolDown();
    }
    else if (dynamic_cast<const Trap*>(&entity)) {
        oss << "type:TRAP";
    }
    
    return oss.str();
}