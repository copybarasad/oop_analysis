#include "EnemyTower.h"
#include "Game.h"
#include <cmath>

EnemyTower::EnemyTower(int health, int damage, int radius, int x, int y, int cooldown)
        : Entity(x, y),
          health_(health),
          damage_(damage),
          radius_(radius),
          initial_cooldown_(cooldown),
          current_cooldown_(0) {}

void EnemyTower::update(Game& game) {
    if (current_cooldown_ > 0) {
        current_cooldown_--;
        return;
    }

    const auto& player = game.getPlayer();
    int dx = player.getX() - x_;
    int dy = player.getY() - y_;

    if (std::abs(dx) <= radius_ && std::abs(dy) <= radius_) {
        // Игрок в радиусе атаки
        game.damagePlayer(damage_);
        game.setLastMessage(game.getLastMessage() + " An enemy tower shoots you!");
        current_cooldown_ = initial_cooldown_;
    }
}

void EnemyTower::takeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) {
        health_ = 0;
    }
}

bool EnemyTower::isDestroyed() const {
    return health_ <= 0;
}

int EnemyTower::getHealth() const {
    return health_;
}