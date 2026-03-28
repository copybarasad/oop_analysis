#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "Trap.h"

class Game;

class EntityManager {
public:
    EntityManager();
    
    void createPlayer(int x, int y, int health, int melee, int ranged);
    void addEnemy(std::unique_ptr<Enemy> enemy);
    void addAlly(std::unique_ptr<Entity> ally);
    void addBuilding(std::unique_ptr<EnemyBuilding> building);
    void addTower(std::unique_ptr<EnemyTower> tower);
    void addTrap(std::unique_ptr<Trap> trap);

    void updateAll(Game& game);
    void removeDeadEntities();
    
    void checkTrapsFor(Game& game, Entity* entity);
    
    void clearNonPlayerEntities();
    void clearAll();

    Player* getPlayer() const;
    Entity* getEntityAt(const Point& pos) const;
    Trap* getTrapAt(const Point& pos) const;
    
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;
    const std::vector<std::unique_ptr<Ally>>& getAllies() const;
    const std::vector<std::unique_ptr<EnemyBuilding>>& getBuildings() const;
    const std::vector<std::unique_ptr<EnemyTower>>& getTowers() const;
    const std::vector<std::unique_ptr<Trap>>& getTraps() const;

private:
    std::unique_ptr<Player> player_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<EnemyBuilding>> buildings_;
    std::vector<std::unique_ptr<Ally>> allies_;
    std::vector<std::unique_ptr<EnemyTower>> towers_;
    std::vector<std::unique_ptr<Trap>> traps_;
};

#endif