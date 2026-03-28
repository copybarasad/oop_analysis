#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameField.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <vector>
#include <random>

class GameState {
public:
    int turnCount = 0;
    GameField field;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;
    int playerHealth = 10000;
    int playerGold = 100;
    int level = 1;
    bool gameRunning = false;
    int playerX = 0;
    int playerY = 0;

    GameState();  // Только объявление

    ~GameState() = default;

    GameState(const GameState& other) = default;
    GameState& operator=(const GameState& other) = default;
    GameState(GameState&& other) = default;
    GameState& operator=(GameState&& other) = default;

    void reset();
    bool initialize();
    bool initializeLevel(int newLevel);

    std::vector<Enemy>& getEnemies() { return enemies; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::vector<EnemyTower>& getTowers() { return towers; }
    const std::vector<EnemyTower>& getTowers() const { return towers; }

    GameField& getField() { return field; }
    const GameField& getField() const { return field; }

private:
    void initializeObstacles();
    bool isPositionOccupied(int x, int y) const;
    bool isNearPlayerStart(int x, int y) const;
    std::mt19937 m_randomGen;
};

#endif // GAMESTATE_H