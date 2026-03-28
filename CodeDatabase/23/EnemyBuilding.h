#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

#include "Entity.h"
#include <string>

class Enemy;

class EnemyBuilding : public Entity {
private:
    std::string enemy_spawn_name;
    int enemy_hp;
    int enemy_dmg;
    int spawn_cooldown;
    int current_cooldown;

public:
    EnemyBuilding(int x, int y, const std::string& enemy_spawn_name, 
                  int enemy_hp, int enemy_dmg, int spawn_cooldown, int hp);
    
    std::string getEnemySpawnName() const;
    int getEnemyHp() const;
    int getEnemyDamage() const;
    int getSpawnCooldown() const;
    int getCurrentCooldown() const;
    bool tick();
    Enemy* spawnEnemy();
};

#endif 
