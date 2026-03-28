#include "enemy_tower.h"
#include <cmath>
#include "entity_type.h"

EnemyTower::EnemyTower(int start_x, int start_y, int level, int interval) 
    : Entity(EntityType::TOWER, start_x, start_y, 80 + 20 * level, 0),
      count(0), 
      interval(interval){
}


void EnemyTower::performAttack(GameMap& map, Position& playerPos) {
    if((++count) >= interval && spell.cast(map, position, playerPos)){
        count = 0;
    }     
}

std::string EnemyTower::getStatusString() const {
    return "Башня | HP: " + std::to_string(health);
}