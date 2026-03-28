#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "player.h"
#include "game_field.h"
#include "enemy.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "trap.h"
#include <vector>
#include <memory>

class GameState {
private:
    int currentLevel;
    int totalScore;
    GameField field;
    Player player;
    std::vector<Enemy> enemies;
    EnemyBuilding enemyBuilding;
    EnemyTower enemyTower;
    std::vector<Trap> traps;
    int enemiesDefeated;

public:
    GameState(int fieldWidth, int fieldHeight, int level = 1);
    
    int getCurrentLevel() const;
    int getTotalScore() const;
    GameField& getField();
    const GameField& getField() const;
    Player& getPlayer();
    const Player& getPlayer() const;
    std::vector<Enemy>& getEnemies();
    const std::vector<Enemy>& getEnemies() const;
    EnemyBuilding& getEnemyBuilding();
    const EnemyBuilding& getEnemyBuilding() const;
    EnemyTower& getEnemyTower();
    const EnemyTower& getEnemyTower() const;
    std::vector<Trap>& getTraps();
    const std::vector<Trap>& getTraps() const;
    int getEnemiesDefeated() const;
    
    void setCurrentLevel(int level);
    void setTotalScore(int score);
    void setEnemiesDefeated(int count);
    
    void resetForNewLevel(int width, int height);
    bool isLevelCompleted() const;
    bool isGameOver() const;
};

#endif