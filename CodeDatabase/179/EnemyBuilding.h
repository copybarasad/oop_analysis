#ifndef ENEMYBUIDING_H
#define ENEMYBUIDING_H

#include "GameObject.h"

class EnemyBuilding : public GameObject {
private:
    int spawnCounter;
    int spawnInterval;

public:
    EnemyBuilding(int health = 100, int spawnInterval = 5,
                  const std::string& name = "Enemy Building");
    
    bool shouldSpawnEnemy() const;
    void update();
    void resetCounter();
    
    std::string getType() const override;
};

#endif
