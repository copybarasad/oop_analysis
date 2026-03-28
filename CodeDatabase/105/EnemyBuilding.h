#pragma once

class EnemyBuilding {
private:
    int spawnCounter;
    int spawnInterval;

public:
    EnemyBuilding(int interval = 5);
    
    bool shouldSpawnEnemy();
    int getSpawnInterval() const;
    int getSpawnCounter() const;

    void setSpawnCounter(int counter);
};
