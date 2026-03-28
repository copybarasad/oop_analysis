#include "Ally.h"
#include "Field.h"
#include "Enemy.h"
#include <cmath>
#include <limits>

Ally::Ally(int startX, int startY) : x(startX), y(startY), health(20), damage(10) {}

bool Ally::isAlive() const { return health > 0; }
int Ally::getX() const {return x;}
int Ally::getY() const {return y;}

void Ally::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Ally::processTurn(Field& field, std::vector<Enemy>& enemies) {
    if (enemies.empty()) return;

    Enemy* target = nullptr;
    double min_dist = std::numeric_limits<double>::max();

    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            double dist = std::hypot(enemy.getX() - x, enemy.getY() - y);
            if (dist < min_dist) {
                min_dist = dist;
                target = &enemy;
            }
        }
    }

    if (!target) return;

    int targetX = target->getX(), targetY = target->getY();
    int nextX = x, nextY = y;

    if (std::abs(targetX - x) > std::abs(targetY - y)) {
        if (targetX > x) nextX++; else nextX--;
    } else {
        if (targetY > y) nextY++; else nextY--;
    }

    if (field.isValidPosition(nextX, nextY)) {
        if (nextX == targetX && nextY == targetY) {
            target->takeDamage(damage);
        } else if (field.getCellType(nextX, nextY) == CellType::EMPTY) {
            move(nextX, nextY, field);
        }
    }
}

void Ally::move(int newX, int newY, Field& field) {
    field.setCellType(x, y, CellType::EMPTY);
    x = newX; y = newY;
    field.setCellType(x, y, CellType::ALLY);
}