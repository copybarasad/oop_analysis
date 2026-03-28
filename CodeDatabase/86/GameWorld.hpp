#pragma once

#include "Grid.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "MainEnemy.hpp"
#include "EnemyConstruction.hpp"
#include <vector>
#include <memory>

struct GameWorld {
    Grid grid_;
    Player player_;
    MainEnemy mainEnemy_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::unique_ptr<EnemyConstruction> tower_;

    GameWorld(Grid&& gridInit, Player&& playerInit, MainEnemy&& mainEnemyInit)
        : grid_(std::move(gridInit)),
          player_(std::move(playerInit)),
          mainEnemy_(std::move(mainEnemyInit)) {}

    Grid& getGrid() { return grid_; }
    const Grid& getGrid() const { return grid_; }
    Player& getPlayer() { return player_; }
    const Player& getPlayer() const { return player_; }
    MainEnemy& getMainEnemy() { return mainEnemy_; }
    const MainEnemy& getMainEnemy() const { return mainEnemy_; }
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return enemies_; }
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies_; }

    EnemyConstruction* getConstruction() { return tower_.get(); }
    const EnemyConstruction* getConstruction() const { return tower_.get(); }
    void setConstruction(std::unique_ptr<EnemyConstruction>&& tower) { tower_ = std::move(tower); }

    void addEnemy(std::unique_ptr<Enemy> enemy) { enemies_.push_back(std::move(enemy)); }
};
