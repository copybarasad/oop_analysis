#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "player.h"
#include "enemy.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "follower.h"  
#include "game_map.h"
#include <vector>

class EntityManager {
private:
    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<EnemyBuilding*> buildings;
    std::vector<EnemyTower*> towers;
    std::vector<Follower*> allies;
public:
    EntityManager();
    ~EntityManager();
    
    Player& getPlayer();
    const Player& getPlayer() const;
    
    std::vector<Enemy*>& getEnemies();
    const std::vector<Enemy*>& getEnemies() const;
    
    std::vector<EnemyBuilding*>& getBuildings();
    const std::vector<EnemyBuilding*>& getBuildings() const;

    std::vector<EnemyTower*>& getTowers();
    const std::vector<EnemyTower*>& getTowers() const;

    std::vector<Follower*>& getFollowers();
    const std::vector<Follower*>& getFollowers() const;

    void entityNew();
    void createPlayer(int x, int y, int health);
    void removeDeadEnemies(GameMap& map);
    void removeDeadTower(GameMap& map);
    void removeDeadAllies(GameMap& map);  
    bool areAllEnemiesDead() const;
    void updateBuildings(GameMap& map);
    void updateEnemies(GameMap& map);
    void updateTower(GameMap& map);
    void updateAllies(GameMap& map);  
    void movePlayer(int dx, int dy, GameMap& map);
    bool summonFollower(const Position& pos, GameMap& map);
    Enemy* findClosestEnemy(const Position& fromPos);  
};

#endif