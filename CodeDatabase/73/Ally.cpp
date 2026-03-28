#include "Ally.h"
#include "Game.h"
#include <cmath>

Ally::Ally(int health, int damage, int x, int y) : Creature(health, damage, x, y) {}

void Ally::update(Game& game) {
    const auto& enemies = game.getEnemies();
    if (enemies.empty()) {
        return;
    }

    const Enemy* target = nullptr;
    int min_dist_sq = 10000;
    for (const auto& enemy : enemies) {
        int dist_sq = (enemy.getX() - x_) * (enemy.getX() - x_) + (enemy.getY() - y_) * (enemy.getY() - y_);
        if (dist_sq < min_dist_sq) {
            min_dist_sq = dist_sq;
            target = &enemy;
        }
    }

    if (!target) return;

    int dx = target->getX() - x_;
    int dy = target->getY() - y_;

    if (std::abs(dx) <= 1 && std::abs(dy) <= 1) {
        game.damageEntityAt(target->getX(), target->getY(), damage_);
        return;
    }

    int move_dx = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int move_dy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;

    int next_x = x_ + move_dx;
    int next_y = y_ + move_dy;

    if (game.canPlaceEntityAt(next_x, next_y)) {
        setPosition(next_x, next_y);
    }
}