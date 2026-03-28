#pragma once

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "Trap.h"
#include "PlayerHand.h"
#include "Spell.h"
#include <vector>
#include <memory>
#include <string>

class GameController {
private:
    GameField field;
    Player player;
    PlayerHand playerHand;
    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> buildings;
    std::vector<EnemyTower> towers;
    std::vector<Ally> allies;
    std::vector<Trap> traps;
    bool gameOver;
    bool gameWon;
    std::string lastMessage;

    void spawnInitialEnemies();
    void spawnInitialBuildings();
    void spawnInitialTowers();
    void moveEnemies();
    void moveAllies();
    void updateBuildings();
    void updateTowers();
    void checkTraps();
    bool tryMovePlayer(int dx, int dy);
    void checkPlayerEnemyCollision();
    void checkAllyEnemyCollision();
    Position findSpawnPositionNearBuilding(const EnemyBuilding& building);
    void removeDeadEntities();

public:
    void spawnEnemiesWithStats(int count, int health, int damage);
    void spawnBuildingsWithStats(int count, int health);
    void spawnTowersWithStats(int count, int health, int damage);
    
    void clearAllEntities();
    GameController(int fieldWidth = 15, int fieldHeight = 15, int handSize = 5);

    bool movePlayerUp();
    bool movePlayerDown();
    bool movePlayerLeft();
    bool movePlayerRight();
   
    void switchPlayerCombatMode();
    bool attackAtPosition(const Position& target);
    
    SpellResult castSelectedSpell(const Position& target);
    void selectNextSpell();
    void selectPreviousSpell();
    bool selectSpell(int index);
    
    void updateGame();

    bool damageEnemyAt(const Position& pos, int damage);
    bool damageBuildingAt(const Position& pos, int damage);
    bool damageTowerAt(const Position& pos, int damage);
    bool canPlaceTrap(const Position& pos) const;
    void placeTrap(const Position& pos, int damage);
    int summonAllies(int count);

    bool isGameOver() const;
    bool isGameWon() const;
    const Player& getPlayer() const;
    Player& getPlayerMutable();
    const GameField& getField() const;
    const std::vector<Enemy>& getEnemies() const;
    std::vector<Enemy>& getEnemiesMutable();
    const std::vector<EnemyBuilding>& getBuildings() const;
    std::vector<EnemyBuilding>& getBuildingsMutable();
    const std::vector<EnemyTower>& getTowers() const;
    std::vector<EnemyTower>& getTowersMutable();
    const std::vector<Ally>& getAllies() const;
    const std::vector<Trap>& getTraps() const;
    const PlayerHand& getPlayerHand() const;
    PlayerHand& getPlayerHandMutable();
    const std::string& getLastMessage() const;
    void setLastMessage(const std::string& msg);
    
    void restorePlayerHealth();
    void setPlayerStats(int maxHealth, int meleeDamage, int rangedDamage);
    
    GameField& getFieldMutable();
};
