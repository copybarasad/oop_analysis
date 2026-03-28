#ifndef PLAYER_ACTION_SYSTEM_H
#define PLAYER_ACTION_SYSTEM_H

#include "Player.h"
#include "GameField.h"
#include "MovementSystem.h"
#include "CombatSystem.h"
#include "EnemyAISystem.h"
#include "BuildingManager.h"
#include "AllyManager.h"
#include "TrapManager.h"
#include "GameNotifier.h"
#include <vector>

class GameController;

class PlayerActionSystem {
    Player& player;
    GameField& field;
    std::vector<Enemy>& enemies;
    std::vector<Trap>& traps;
    std::vector<Ally>& allies;
    std::vector<EnemyTower>& towers;
    std::vector<EnemyBuilding>& buildings;
    
    MovementSystem& movementSystem;
    CombatSystem& combatSystem;
    EnemyAISystem& enemyAISystem;
    BuildingManager& buildingManager;
    AllyManager& allyManager;
    IGameNotifier& notifier;

public:
    PlayerActionSystem(Player& player, GameField& field,
                      std::vector<Enemy>& enemies, std::vector<Trap>& traps,
                      std::vector<Ally>& allies, std::vector<EnemyTower>& towers,
                      std::vector<EnemyBuilding>& buildings,
                      MovementSystem& movementSystem, CombatSystem& combatSystem,
                      EnemyAISystem& enemyAISystem, BuildingManager& buildingManager,
                      AllyManager& allyManager,
                      IGameNotifier& notifier);  
    
    void processMove(const Position &direction);
    void processAttack(const Position &direction);
    void processAreaAttack();
    void processSwitchCombat();
    void processEnemyTurns();
    void updateBuildings();
    void removeDeadEnemies();
    void checkCollisions();
    
private:
    void processPlayerRangedAttack(const Position &direction);
    Enemy* getEnemyAtPosition(const Position &position);
    EnemyTower* getTowerAtPosition(const Position &position);
};

#endif