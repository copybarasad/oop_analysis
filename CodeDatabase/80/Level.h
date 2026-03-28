#ifndef LEVEL_H
#define LEVEL_H

#include "Map.h"
#include <string>

class Level {
private:
    int levelNumber;
    int mapWidth;
    int mapHeight;
    int towerHealth;
    int attackTowerHealth;
    int enemyDamage;
    int enemyHealth;
    std::string levelName;

public:
    Level(int number, int width, int height, int tHealth, int atHealth, int eDamage, int eHealth, const std::string& name);

    int GetLevelNumber() const;
    int GetMapWidth() const;
    int GetMapHeight() const;
    int GetTowerHealth() const;
    int GetAttackTowerHealth() const;
    int GetEnemyDamage() const;
    int GetEnemyHealth() const;
    const std::string& GetLevelName() const;

    Map CreateMap() const;
};

#endif