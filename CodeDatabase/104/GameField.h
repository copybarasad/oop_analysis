#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Position.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <vector>
#include <memory>
#include <random>

class Player;

class GameField {
private:
    int width_;
    int height_;
    Player* player_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<EnemyTower>> towers_;
    std::vector<Position> trap_positions_;
    std::vector<Position> obstacles_;
    mutable std::mt19937 random_generator_;

public:
    GameField(int width, int height, bool generate_obstacles);

    // Основные методы
    bool movePlayer(int target_x, int target_y);
    const Position& getPlayerPosition() const;
    void setPlayer(Player* player) { player_ = player; }

    bool isPositionValid(int x, int y) const;
    bool isPositionOccupied(int x, int y) const;
    bool hasTrapAt(int x, int y) const;
    bool hasObstacleAt(int x, int y) const;
    bool isPassable(int x, int y) const;

    void spawnEnemies(int count, int base_health, int base_damage);
    void updateEnemies();
    int getEnemyCount() const;
    bool areAllEnemiesDefeated() const;
    bool hasEnemyAt(int x, int y) const;

    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies_; }
    const std::vector<std::unique_ptr<EnemyTower>>& getTowers() const { return towers_; }
    const std::vector<Position>& getTraps() const { return trap_positions_; }
    const std::vector<Position>& getObstacles() const { return obstacles_; }
    void addObstacle(const Position& position) { obstacles_.push_back(position); }
    void clearObstacles() { obstacles_.clear(); }

    // Методы для добавления конкретных врагов и башен
    void addEnemy(std::unique_ptr<Enemy> enemy) {
        enemies_.push_back(std::move(enemy));
    }
    void addTower(std::unique_ptr<EnemyTower> tower) {
        towers_.push_back(std::move(tower));
    }
    void addTrap(const Position& position) {
        trap_positions_.push_back(position);
    }
    void clearEnemies() { enemies_.clear(); }
    void clearTowers() { towers_.clear(); }
    void clearTraps() { trap_positions_.clear(); }
    void spawnTowers(int count);
    void updateTowers();
    int getTowerCount() const;
    bool hasTowerAt(int x, int y) const;

    bool canPlaceTrapAt(int x, int y) const;
    void placeTrap(const Position& position, int damage);
    void damageAtPosition(const Position& position, int damage);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    Position getRandomEmptyPosition() const;
    Position getRandomEmptyPositionFarFromPlayer(int min_distance) const;
    void generateObstacles();
    void moveEnemyTowardsPlayer(Enemy& enemy);
    void handleEnemyAttack(Enemy& enemy);
    void checkEnemyTraps();
};

#endif