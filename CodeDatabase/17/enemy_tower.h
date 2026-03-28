#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "entity.h"
#include "position.h"
#include "direct_damage_spell.h"

class EnemyTower : public Entity {
private:
    int count;
    int interval;
    DirectDamageSpell spell = DirectDamageSpell(5, 3);
    
public:
    EnemyTower(int start_x, int start_y, int level = 1, int interval = 5);
    void performAttack(GameMap& map, Position& playerPos);
    
    std::string getStatusString() const override;
};

#endif