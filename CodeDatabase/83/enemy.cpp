#include "enemy.h"
#include "player.h"
#include "game_field.h"
#include "game_defines.h"
#include <cmath>
#include <cstdlib>

Enemy::Enemy(int x, int y) : health(ENEMY_HEALTHB), damage(ENEMY_DAMAGEB), x(x), y(y) {}

void Enemy::move(Player& player, std::vector<Enemy>& allEnemies, GameField& field) {
    if (!isAlive()) return;
    
    int oldX = x;
    int oldY = y;
    
    int dx = player.getX() - x;
    int dy = player.getY() - y;
    
    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0 && field.canMoveTo(player, allEnemies, x + 1, y)) x++;
        else if (dx < 0 && field.canMoveTo(player, allEnemies, x - 1, y)) x--;
    } else {
        if (dy > 0 && field.canMoveTo(player, allEnemies, x, y + 1)) y++;
        else if (dy < 0 && field.canMoveTo(player, allEnemies, x, y - 1)) y--;
    }
    
    if (x == oldX && y == oldY) {
        if (dx != 0 && field.canMoveTo(player, allEnemies, x + (dx > 0 ? 1 : -1), y)) {
            x += (dx > 0 ? 1 : -1);
        } else if (dy != 0 && field.canMoveTo(player, allEnemies, x, y + (dy > 0 ? 1 : -1))) {
            y += (dy > 0 ? 1 : -1);
        }
    }
    
    for (auto& other : allEnemies) {
        if (&other != this && other.isAlive() && other.getX() == x && other.getY() == y) {
            x = oldX;
            y = oldY;
            break;
        }
    }
}

void Enemy::attack(Player& player) const {
    if (isAlive()) {
        player.takeDamage(damage);
    }
}

void Enemy::takeDamage(int damage) {
    health = std::max(0, health - damage);
}

bool Enemy::isAlive() const {
    return health > 0;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getX() const {
    return x;
}

int Enemy::getY() const {
    return y;
}

int Enemy::getDamage() const {
    return damage;
}

void Enemy::setHealth(int newHealth) {
    health = newHealth;
    if (health < 0) health = 0;
}

void Enemy::setDamage(int newDamage) {
    damage = newDamage;
}