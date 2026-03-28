#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include <vector>
#include "entity_type.h"

class GameMap;
class Position;

class Enemy : public Entity {
public:
    Enemy(int start_x = 0, int start_y = 0, int level = 1, 
          int enemy_health = 50, int enemy_damage = 10)
        : Entity(EntityType::ENEMY, start_x, start_y, enemy_health + 20 * (level - 1), enemy_damage  + 10 * (level - 1), false) {}
    
    ~Enemy() = default;
};

#endif