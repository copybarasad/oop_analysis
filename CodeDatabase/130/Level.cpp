#include "Level.h"

Level::Level(int levelNum, int width, int height, int enemyHP, int enemyDmg,
             int enemyCount, int buildingCount, int towerCount, int buildingCooldown)
    : levelNumber_(levelNum), fieldWidth_(width), fieldHeight_(height),
      enemyHP_(enemyHP), enemyDamage_(enemyDmg), enemyCount_(enemyCount),
      buildingCount_(buildingCount), towerCount_(towerCount),
      buildingCooldown_(buildingCooldown) {}

Level Level::generateLevel(int levelNum) {
    if (levelNum < 1) levelNum = 1;
    if (levelNum > 10) levelNum = 10;
    
    int width = 15 + (levelNum - 1) * 1;
    int height = 15 + (levelNum - 1) * 1;
    int enemyHP = 40 + (levelNum - 1) * 10;
    int enemyDamage = 5 + (levelNum - 1) * 2;
    int enemyCount = 3 + (levelNum - 1) * 1;
    int buildingCount = 1;
    int towerCount = 1 + (levelNum - 1) / 3;
    int buildingCooldown = 9;
    
    return Level(levelNum, width, height, enemyHP, enemyDamage, 
                 enemyCount, buildingCount, towerCount, buildingCooldown);
}
