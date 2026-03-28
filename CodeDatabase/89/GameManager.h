#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameGrid.h"
#include "MovementSystem.h"
#include "CombatSystem.h"
#include "SpawnSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <vector>
#include <memory>

class GameManager {
public:
    GameManager(int width, int height, int impassableCellsCount);
    
    void placePlayer(std::unique_ptr<Player> player);
    void addEnemy(std::unique_ptr<Enemy> enemy);
    void addTower(std::unique_ptr<EnemyTower> tower);
    void clearEnemiesAndTowers();
    
    bool movePlayer(Direction direction);
    void moveEnemies();
    void playerAttack(Direction direction);
    bool castPlayerSpell(int spellIndex, Direction direction);

    void processTowerAttacks();
    void updateTowers();

    void printField() const;
    
    Player* getPlayer() const;
    const Player* getPlayerConst() const { return player.get(); }
    std::unique_ptr<Player> getPlayerPtr();
    size_t getEnemyCount() const;
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies; }
    const std::vector<std::unique_ptr<EnemyTower>>& getTowers() const { return towers; }
    GameGrid& getGridRef() { return grid; }
    const GameGrid& getGridRef() const { return grid; }  

    bool isGameOver() const;
    bool isGameWon() const;

private:
    GameGrid grid;
    MovementSystem movementSystem;
    CombatSystem combatSystem;
    SpawnSystem spawnSystem;
    
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<EnemyTower>> towers;

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    GameManager(GameManager&&) = default;
    GameManager& operator=(GameManager&&) = default;
};

#endif