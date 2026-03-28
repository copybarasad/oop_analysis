#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameField.h"
#include "Player.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "GameEnums.h"
#include <vector>
#include <memory>

class GameManager {
private:
    std::unique_ptr<GameField> field;
    std::unique_ptr<Player> player;
    std::vector<std::pair<int, int>> buildingPositions;
    std::vector<std::unique_ptr<EnemyBuilding>> buildings;
    std::vector<std::unique_ptr<EnemyTower>> towers;
    std::pair<int, int> playerPosition;

    bool tryMovePlayer(int newX, int newY);
    bool performMeleeAttack(int targetX, int targetY);
    bool performRangedAttack(Direction direction);
    void spawnNewEnemies();
    void updateAllEnemies(const std::vector<std::pair<int, int>>& newEnemiesThisTurn);
    void updateTowers();
    void updateTraps();
    void updateAllies();
    void checkSpellRewards();
    std::unique_ptr<Spell> createSpellForSlot(int slotIndex);
    void giveNewSpellToPlayer();

public:
    GameManager(int width, int height, int level);
    bool movePlayer(Direction direction);
    bool attackEnemy(Direction direction);
    bool castSpell(int spellIndex, int targetX, int targetY);
    void switchPlayerCombatMode();
    void updateGame();
    void addEnemyBuilding(int x, int y, int spawnInterval, int enemyHealth, int enemyDamage);
    void addEnemyTower(int x, int y, int range);

    const Player& getPlayer() const;
    Player& getPlayer();
    std::pair<int, int> getPlayerPosition() const;
    void setPlayerPosition(int x, int y);
    const GameField& getField() const;
    GameField& getField();
    bool hasBuildingAt(int x, int y) const;
    bool hasTowerAt(int x, int y) const;
    bool isGameOver() const;
    bool purchaseSpell(int cost);

    void damageEnemyAt(int x, int y, int damage);
    bool damageEnemiesInArea(int centerX, int centerY, int areaSize, int damage);
    void placeTrapAt(int x, int y, int damage);
    int summonAlliesNearPlayer(int count);
};

#endif
