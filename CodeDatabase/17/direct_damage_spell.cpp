#include "direct_damage_spell.h"
#include "game_map.h"
#include "entity.h"
#include "player.h"


bool DirectDamageSpell::cast(GameMap& map, Position casterPos, Position target) {
    int distance = abs(casterPos.getX() - target.getX()) + 
                   abs(casterPos.getY() - target.getY());
    MapCell& playerCell = map.getCell(casterPos);
    Entity* entity = playerCell.getEntity();
    if (distance > range * entity->getLevel()) { 
        return false;
    }

    if (!map.isPositionValid(target)) {
        return false;
    }
    MapCell& targetCell = map.getCell(target);
    Entity* targetEntity = targetCell.getEntity();
    
    if (!targetEntity) {
        return false; 
    }
    else{
        targetEntity->takeDamage(damage);
        if (!targetEntity->isAlive()) {
            if (entity) {
                if (!targetEntity->isAlive()) {
                    if(entity->getType() == EntityType::PLAYER && targetEntity->getType() == EntityType::ENEMY){
                        entity->onKill();
                    }
                    targetCell.setEntity(nullptr);
                    targetCell.setUsed(false);
                }
            }
        }
    }

    return true;
}