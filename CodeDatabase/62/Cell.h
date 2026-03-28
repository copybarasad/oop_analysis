#pragma once
#include "SpawnerBuilding.h"
#include "Enemy.h"
#include "Player.h"
#include "Tower.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>

class Cell {
private:
    char cell_;
    Enemy* enemy_ = nullptr;
    Enemy* ally_ = nullptr;
    Player* player_ = nullptr;
    SpawnerBuilding* spawner_building_ = nullptr;
    Tower* tower_ = nullptr;
    int trap_ = 0;

    sf::Color color_;
    float color_timer_ = 0.0f;
    float max_timer_ = 1.0f;

public:
    Cell(char cell): cell_(cell), color_(sf::Color::Transparent)  {}

    Cell(const Cell& other) : cell_(other.cell_), trap_(other.trap_) {
        if(other.enemy_ != nullptr) {
            enemy_ = new Enemy(*other.enemy_);
        }
        if(other.player_ != nullptr) {
            player_ = new Player(*other.player_);
        }
        if(other.spawner_building_ != nullptr) {
            spawner_building_ = new SpawnerBuilding(*other.spawner_building_);
        }
        if(other.ally_ != nullptr) {
            ally_ = new Enemy(*other.ally_);
        }
        if(other.tower_ != nullptr) {
            tower_ = new Tower(*other.tower_);
        }
    }

    Cell(Cell&& other) noexcept:
        cell_(other.cell_),
        enemy_(other.enemy_),
        ally_(other.ally_),
        player_(other.player_),
        spawner_building_(other.spawner_building_),
        tower_(other.tower_),
        trap_(other.trap_),
        color_(other.color_),
        color_timer_(other.color_timer_),
        max_timer_(other.max_timer_) {
        other.enemy_ = nullptr;
        other.player_ = nullptr;
        other.spawner_building_ = nullptr;
        other.ally_ = nullptr;
        other.cell_ = ' ';
        other.tower_ = nullptr;
        other.trap_ = false;
        other.color_ = sf::Color::Transparent;
        other.color_timer_ = 0.0f;
        other.max_timer_ = 1.0f;
    }

    Cell& operator=(Cell&& other) noexcept {
        if (this != &other) {
            cell_ = other.cell_;
            enemy_ = other.enemy_;
            player_ = other.player_;
            ally_ = other.ally_;
            spawner_building_ = other.spawner_building_;
            tower_ = other.tower_;
            trap_ = other.trap_;
            color_ = other.color_;
            color_timer_ = other.color_timer_;
            max_timer_ = other.max_timer_;
            other.enemy_ = nullptr;
            other.player_ = nullptr;
            other.spawner_building_ = nullptr;
            other.ally_ = nullptr;
            other.cell_ = ' ';
            other.tower_ = nullptr;
            other.trap_ = false;
            other.color_ = sf::Color::Transparent;
            other.color_timer_ = 0.0f;
            other.max_timer_ = 1.0f;
        }
        return *this;
    }

    Cell& operator=(const Cell& other) {
        if (this != &other) {
            cell_ = other.cell_;
            trap_ = other.trap_;
            color_ = other.color_;
            color_timer_ = other.color_timer_;
            max_timer_ = other.max_timer_;
            if(other.ally_ != nullptr) {
                ally_ = new Enemy(*ally_);
            }
            if(other.enemy_ != nullptr) {
                enemy_ = new Enemy(*other.enemy_);
            }
            if(other.player_ != nullptr) {
                player_ = new Player(*other.player_);
            }
            if(other.spawner_building_ != nullptr) {
                spawner_building_ = new SpawnerBuilding(*other.spawner_building_);
            }
            if(other.tower_ != nullptr) {
                tower_ = new Tower(*other.tower_);
            }
        }
        return *this;
    }

    ~Cell() = default;

    Enemy* getEnemy() const { return enemy_; }
    Enemy* getAlly() const { return ally_; }
    Player* getPlayer() const { return player_; }
    SpawnerBuilding* getSpawnerBuilding() const { return spawner_building_; }
    Tower* getTower() const { return tower_; }
    char getCell() const { return cell_; }
    bool isFree() const { return cell_ != TILE_WALL && enemy_ == nullptr && player_ == nullptr && spawner_building_ == nullptr && ally_ == nullptr && tower_ == nullptr; }

    void setEnemy(Enemy* e) { enemy_ = e; };
    void setAlly(Enemy* a) { ally_ = a; }
    void setPlayer(Player* p) { player_ = p; };
    void setSpawnerBuilding(SpawnerBuilding* b) { spawner_building_ = b; };
    void setCell(char c) { cell_ = c; };
    void setTower(Tower* t) { tower_ = t; }

    void installTrap(int v) { trap_ = v; }
    void removeTrap() { trap_ = 0; }
    int getTrap() const { return trap_; }

    void setColor(sf::Color color, float timer) { color_ = color; color_timer_ = timer; }
    sf::Color getColor() const { return color_; }

    void update(float dt) {
        if(color_timer_ > 0.0f) {
            color_timer_ = std::max(0.0f, color_timer_ - dt);
            color_.a = color_timer_ / max_timer_ * 255;
            if(color_timer_ <= 0.0f) {
                color_ = sf::Color::Transparent;
            }
        }
    }
};