#pragma once
#include <vector>
#include <memory>
#include "Cell.h"

class Field {
private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;

public:
    bool inBounds(int x, int y) const;

    Field(int width = 15, int height = 15);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    int getWidth() const;
    int getHeight() const;
    const Cell& getCell(int x, int y) const;
    Cell& getCell(int x, int y);

    bool movePlayer(int dx, int dy);
    bool rangedAttack(int dx, int dy, int range);
    void updateBases();
    void initializeRandomObstacles(double wallProb = 0.1, double slowProb = 0.05);

    std::pair<int, int> findPlayer() const;
    bool isPlayerOnField() const;
    std::vector<std::pair<int, int>> findEnemies() const;
    std::vector<std::pair<int, int>> findBases() const;
    std::vector<std::pair<int, int>> findTowers() const;
    std::vector<std::pair<int, int>> findAllies() const;
    std::pair<int, int> findTowerPosition(const EnemyTower* tower) const;

    void placeTrap(int x, int y);
    void placeAlly(int x, int y, std::shared_ptr<Ally> ally);
    void moveEnemies();
};