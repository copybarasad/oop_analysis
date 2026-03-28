#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "Ally.h"
#include "EventLogger.h"

class GameController; 

class GameLevel {
public:
    GameLevel(int width, int height);
    void update(Player& player, GameController& game, EventLogger* logger);
    
    void cleanupDead(Player& player, EventLogger* logger);

    void damageAt(int x, int y, int damage);
    void placeTrap(int x, int y, int damage);
    bool spawnAllyNear(int x, int y);
    
    bool isCleared() const;
    
    Field& getField();
    const Field& getField() const;

    const std::vector<Enemy>& getEnemies() const;
    const std::vector<EnemySpawner>& getSpawners() const;
    const std::vector<Trap>& getTraps() const;
    const std::vector<Ally>& getAllies() const;
    const std::vector<EnemyTower>& getTowers() const;

    std::vector<Enemy>& getEnemiesEx();
    std::vector<EnemySpawner>& getSpawnersEx();
    std::vector<Trap>& getTrapsEx();
    std::vector<Ally>& getAlliesEx();
    std::vector<EnemyTower>& getTowersEx();

    void clearEntities();

private:
    Field field;
    std::vector<Enemy> enemies;
    std::vector<EnemySpawner> spawners;
    std::vector<Trap> traps;
    std::vector<Ally> allies;
    std::vector<EnemyTower> towers;
};

#endif