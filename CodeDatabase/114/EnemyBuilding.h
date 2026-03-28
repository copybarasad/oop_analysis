#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

#include "Enemy.h"

class EnemyBuilding : public Entity {
private:
    int turnsToSpawn;
    int maxTurnsToSpawn;
public:
    EnemyBuilding(int posX, int posY, int turnsToSpawn = 3, int buildingHealth = 100);
    int getDamage() const { return 0; }
    void update();
    bool canSpawnEnemy();
    
    int getTurnsToSpawn() const { return turnsToSpawn; }
    void setTurnsToSpawn(int turns) { turnsToSpawn = turns; }
    int getMaxTurnsToSpawn() const { return maxTurnsToSpawn; }
    void setMaxTurnsToSpawn(int turns) { maxTurnsToSpawn = turns; }
    
    std::string getEntityName() const override { return "Вражеское здание"; }
};

#endif
