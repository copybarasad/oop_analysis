#pragma once

#include "EnemyBuilding.h"

class EnemySpawner : public EnemyBuilding 
{
protected:
    int spawnCD;
    int currentCD;
public:
    EnemySpawner(int X_coord = 0, int Y_coord = 0, int spawnCD = 5);
    bool updateAndSpawn();
    std::string serialize() const override;
    void deserialize(const std::string& data) override;
};