#pragma once
#include <vector>
#include <utility>
#include "Cell.h"

class GameField {
private:
    int width_;
    int height_;
    int level_;
    std::vector<std::vector<Cell>> cells_;

public:
    std::function<void(int, int, int, float)> applyDamage;
    GameField(int width, int height, int level,
    std::function<void(int, int, int, float)> applyDamage);

    GameField(GameField& other) : width_(other.width_), height_(other.height_), cells_(other.cells_) {}

    GameField(GameField&& other) : width_(other.width_), height_(other.height_), cells_(std::move(other.cells_)) {
        other.width_ = 0;
        other.height_ = 0;
    }

    GameField& operator=(GameField&& other) noexcept {
        if (this == &other) return *this;

        width_ = other.width_;
        height_ = other.height_;
        cells_ = std::move(other.cells_);
        level_ = other.level_;

        other.width_ = 0;
        other.height_ = 0;
        return *this;
    }

    GameField& operator=(const GameField& other) {
        if (this == &other) return *this;

        width_ = other.width_;
        height_ = other.height_;
        cells_ = other.cells_;
        level_ = other.level_;
        return *this;
    }

    ~GameField();

    std::pair<int, int> getFreePosition();

    void addEnemy(Enemy* entity, int x, int y);

    void addAlly(Enemy* e, int x, int y);

    void addPlayer(Player* player, int x, int y);

    void addBuilding(SpawnerBuilding* building, int x, int y);

    void addTower(Tower* tower, int x, int y);

    std::vector<Enemy*> getEnemiesInRow(int x1, int y1, int x2, int y2) const;

    std::vector<Enemy*> getEnemiesInArea(int cx, int cy, int radius) const;
    std::vector<Tower*> getTowersInArea(int cx, int cy, int radius) const;
    std::vector<Enemy*> getAlliesInArea(int cx, int cy, int radius) const;
    std::vector<Entity*> getAlliesInAreaWithPlayer(int cx, int cy, int radius) const;

    void setColorInArea(int cx, int cy, int radius, sf::Color color, float t);

    sf::Color getColorAt(int x, int y) const;

    std::pair<int, int> getClosestEnemy(int x, int y);
    std::pair<int, int> getClosestAllyOrPlayer(int x, int y);

    Enemy* getEnemyAt(int x, int y) const;

    Enemy* getAllyAt(int x, int y) const;

    SpawnerBuilding* getSpawnerBuildingAt(int x, int y) const;

    Tower* getTowerAt(int x, int y) const;

    std::pair<int, int> getEnemyCell(Enemy* entity) const;
    std::pair<int, int> getAllyCell(Enemy* entity) const;
    std::pair<int, int> getTowerCell(Tower* entity) const;
    std::pair<int, int> getPlayerCell() const;

    Player* getPlayer() const;

    void spawnEntity(std::string spawner_name);

    void moveEnemy(Enemy* entity, int x, int y);
    void moveAlly(Enemy* entity, int x, int y);

    void spawnAlly(Enemy* ally);

    void movePlayer(int x, int y);

    void swap(int x1, int y1, int x2, int y2);

    void removeEnemy(Enemy* entity);
    void removeAlly(Enemy* entity);
    void removePlayer();
    void removeTower(Tower* entity);

    void remove(int x, int y);

    char getCell(int x, int y) const;
    
    int getTrap(int x, int y) const;

    bool isFree(int x, int y) const;

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    void makeBuildingsMove();

    void installTrapAt(int x, int y, int v);

    void updateCell(int x, int y, float dt);

    std::vector<std::pair<int, int>> findPath(int x1, int y1, int x2, int y2) const;

    int getLevel() const { return level_; }

    void recreateCell(char c, int x, int y) {
        cells_[y][x] = Cell(c);
    }

    void clearSpawnerBuildings();
};