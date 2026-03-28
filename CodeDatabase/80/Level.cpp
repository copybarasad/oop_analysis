#include "Level.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Level::Level(int number, int width, int height, int tHealth, int atHealth, int eDamage, int eHealth, const std::string& name)
    : levelNumber(number), mapWidth(width), mapHeight(height),
    towerHealth(tHealth), attackTowerHealth(atHealth),
    enemyDamage(eDamage), enemyHealth(eHealth), levelName(name) {

    std::srand(std::time(0));
}

int Level::GetLevelNumber() const { return levelNumber; }
int Level::GetMapWidth() const { return mapWidth; }
int Level::GetMapHeight() const { return mapHeight; }
int Level::GetTowerHealth() const { return towerHealth; }
int Level::GetAttackTowerHealth() const { return attackTowerHealth; }
int Level::GetEnemyDamage() const { return enemyDamage; }
int Level::GetEnemyHealth() const { return enemyHealth; }
const std::string& Level::GetLevelName() const { return levelName; }

Map Level::CreateMap() const {
    Map map(mapWidth, mapHeight);

    if (levelNumber == 1) {
        map.SetVal(1, 1, '*', nullptr);
        map.SetVal(2, 4, '*', nullptr);
        map.SetVal(2, 3, '/', nullptr);
        map.SetVal(3, 3, '/', nullptr);
    }
    else if (levelNumber == 2) {
        map.SetVal(3, 3, '*', nullptr);
        map.SetVal(5, 5, '*', nullptr);
        map.SetVal(7, 2, '*', nullptr);
        map.SetVal(2, 7, '/', nullptr);
        map.SetVal(6, 6, '/', nullptr);
    }
    else {
        for (int i = 0; i < levelNumber + 2; i++) {
            int trapX = std::rand() % (mapHeight - 2) + 1;
            int trapY = std::rand() % (mapWidth - 2) + 1;
            char trapType = (std::rand() % 2 == 0) ? '*' : '/';
            map.SetVal(trapX, trapY, trapType, nullptr);
        }
    }

    return map;
}