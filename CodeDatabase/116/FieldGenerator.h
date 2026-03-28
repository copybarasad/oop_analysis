#pragma once

#include <random>
#include "GameWorld.h"

class FieldGenerator {
private:
    int enemiesCount;
    int wallsCount;
    int slowingCellsCount;
    int fortressesCount;
    int lvl;
public:
    FieldGenerator(int enemyCount, int slowingCellsCount, int wallsCount, int fortressesCount, int lvl = 0);
    ~FieldGenerator();

    int getEnemiesCount() const { return enemiesCount; }
    int getWallsCount() const { return wallsCount; }
    int getSlowingCellsCount() const { return slowingCellsCount; }
    int getFortressesCount() const { return fortressesCount; }

    void spawnStartObjects(GameWorld& world);
    void spawnBuilding(GameWorld& world,int x, int y, int Health = 40, int frequency = 5);
    void spawnEnemy(GameWorld& world,int x, int y, int health = 30, int damage = 8);
    void spawnAlly(GameWorld& world,int x, int y, int health = 50, int damage = 10);
    void spawnTrap(GameWorld& world,int x, int y, int damage);
    void spawnFortress(GameWorld& world,int x, int y, int health = 40, int attackRad = 2, int damage = 4, int attackFrequency = 3);
private:
    std::pair<int, int> getRandomEmptyPosition(GameWorld& world) const;
};