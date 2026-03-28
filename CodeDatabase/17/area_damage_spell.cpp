#include "area_damage_spell.h"
#include "game_map.h"
#include "entity.h"
#include "player.h"

bool AreaDamageSpell::cast(GameMap& map, Position casterPos, Position targetPos) {
    int distance = abs(casterPos.getX() - targetPos.getX()) + 
                   abs(casterPos.getY() - targetPos.getY());
    
    if (!map.isPositionValid(targetPos)) {
        return false;
    }
 
    if (distance > range) { 
        return false;
    }

    MapCell& playerCell = map.getCell(casterPos);
    Entity* entity = playerCell.getEntity();
    
    for (int dx = 0; dx <= 1 * entity->getLevel(); dx++) {
        for (int dy = 0; dy <= 1 * entity->getLevel(); dy++) {
            Position areaPos(targetPos.getX() + dx, targetPos.getY() + dy);
            if (!map.isPositionValid(areaPos)) {
                continue;
            }       
            MapCell& cell = map.getCell(areaPos);
            Entity* targetEntity = cell.getEntity();     
            if (targetEntity) {
                targetEntity->takeDamage(damage);
                
                if (!targetEntity->isAlive()) {
                    cell.setEntity(nullptr);
                    cell.setUsed(false);
                    if(entity->getType() == EntityType::PLAYER && targetEntity->getType() == EntityType::ENEMY){
                        entity->onKill();
                    }
                }
            }
        }
    }

    return true;
}