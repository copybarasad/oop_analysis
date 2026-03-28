#ifndef ENEMY_BUILDING_HPP
#define ENEMY_BUILDING_HPP

#include "game_object.hpp"
#include <vector>
#include <memory>

class Enemy;
class Player;

class EnemyBuilding : public GameObject {
private:
    int spawnCooldown;
    int currentCooldown;
    std::vector<std::unique_ptr<GameObject>>& enemies;
    Player* targetPlayer;
    bool hasSpawnedThisTurn;
    
    static constexpr int SPAWN_COOLDOWN_DEFAULT = 3;

public:
    EnemyBuilding(int startX, int startY, Cell& cell, int hp, int cooldown,
                  std::vector<std::unique_ptr<GameObject>>& enemyList, Player* player);
    
    ~EnemyBuilding() override = default;
    
    void takeTurn(Map& map) override;
    void resetTurnFlags() override;
    
    bool canSpawn() const;
    bool trySpawnEnemy(Map& map);
    void findSpawnPosition(Map& map, int& outX, int& outY) const;
    
    int getSpawnCooldown() const { return spawnCooldown; }
    int getCurrentCooldown() const { return currentCooldown; }
    
private:
    bool isValidSpawnPosition(Map& map, int x, int y) const;
    static void initRandom();
    static bool randomInitialized;
};

#endif