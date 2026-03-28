#pragma once
#include <vector>
#include <memory>
#include <random>
#include <string>
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "Direction.h"
#include "Trap.h"
#include "EnemyTower.h"

class GameField {
private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;
    Position playerPosition;
    std::vector<std::pair<Position, Enemy>> enemies;
    std::vector<std::string> currentTurnLog;
    std::vector<std::string> previousTurnLog;

    std::vector<Trap> traps;
    std::vector<EnemyTower> towers;

    bool isValidPosition(const Position& pos) const;
    int calculateDistance(const Position& a, const Position& b) const;
    Direction getDirectionTowardsPlayer(const Position& enemyPos) const;
    Direction findBestMoveToPlayer(const Position& enemyPos) const;
    Direction getRandomDirection() const;
    bool isPlayerDetected(const Position& enemyPos) const;
    bool isStartValid(const Position& pos) const;

public:
    GameField();
    GameField(int fieldWidth, int fieldHeight);


    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    ~GameField() = default;

    bool placePlayer(const Position& pos);
    bool placeEnemy(const Position& pos, Enemy enemy);
    bool placeObstacle(const Position& pos);
    bool movePlayer(Direction direction, Player& player);
    bool performRangedAttack(Direction direction, Player& player);
    void moveEnemies(Player& player);
    void removeDeadEnemies();

    int getWidth() const;
    int getHeight() const;
    Cell getCell(const Position& pos) const;
    Position getPlayerPosition() const;
    std::vector<Position> getEnemyPositions() const;
    int getEnemyCount() const;
    const std::vector<std::string>& getPreviousTurnLog() const;
    void endTurn();

    bool isPositionEmpty(const Position& pos) const;
    bool isPositionPassable(const Position& pos) const;
    bool checkIsValidPosition(const Position& pos) const;
    Position getRandomEmptyPosition() const;
    bool tryGetRandomEmptyPosition(Position& outPos, int maxAttempts) const;
    void spawnObstacles(double obstacleCoeff = 0.1);
    void ensurePlayerHasFreeNeighbor();
    void addToCurrentTurnLog(const std::string& message);
    
    std::vector<Position> getAreaDamagePositions(const Position& targetPos, 
                                                   Direction direction, int areaSize) const;
    bool damageEnemyAt(const Position& pos, int damage);
    void damageEnemiesInArea(const std::vector<Position>& positions, int damage);
    bool isPositionBlockedByObstacle(const Position& pos) const;

    std::vector<Trap>& getTraps();
    const std::vector<Trap>& getTraps() const;
    bool placeTrap(const Position& pos, int damage);
    void removeDeadTraps();
    
    std::vector<EnemyTower>& getTowers();
    const std::vector<EnemyTower>& getTowers() const;
    bool addTower(const Position& pos, EnemyTower&& tower);
    void removeDeadTowers();
    int checkTowerAttackOnPlayer(const Position& playerPos);
    std::vector<Position> getTowerPositions() const;
    int getTowerCount() const;
    bool damageTowerAt(const Position& pos, int damage);
    bool damageTargetInLine(const Position& startPos, Direction direction, int range, 
                           int damage);
    bool hasTrapAt(const Position& pos) const;
    
    bool addTowerWithState(const Position& pos, EnemyTower&& tower);
    const std::vector<std::pair<Position, Enemy>>& getEnemies() const { return enemies; }
    Enemy* getEnemyAt(const Position& pos);
    const Enemy* getEnemyAt(const Position& pos) const;
};