#ifndef ENEMY_TOWER_HPP
#define ENEMY_TOWER_HPP

#include "field.hpp"
#include "player.hpp"
#include <iostream>
#include <cmath>

class Game;

class EnemyTower {
private:
    int x_;
    int y_;
    int attack_radius_;
    int damage_;
    int cooldown_;
    Game* game_;

public:
    EnemyTower(int x, int y, Game* game,
               int attack_radius = 1, int damage = 10, int cooldown = 1)
        : x_(x), y_(y),
          attack_radius_(attack_radius),
          damage_(damage),
          cooldown_(0),
          game_(game) {}

    int GetX() const noexcept { return x_; }
    int GetY() const noexcept { return y_; }

    bool IsReady() const noexcept { return cooldown_ == 0; }

    void CooldownTick() {
        if (cooldown_ > 0)
            --cooldown_;
    }

    void Tick();

    Game* GetGame() const noexcept { return game_; }
};

#endif