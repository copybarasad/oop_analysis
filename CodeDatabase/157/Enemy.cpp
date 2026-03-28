#include "Enemy.h"
#include "GameField.h"
#include "Player.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(int x, int y, int hp, int damage)
        : Unit(x, y, hp, true), damage_(damage) {}

static int sign(int v) {
    if (v > 0) return 1;
    if (v < 0) return -1;
    return 0;
}

void Enemy::act(GameField& field) {
    Player* player = field.player();
    if (!player) return;

    int px = player->x();
    int py = player->y();

    int dx = px - x();
    int dy = py - y();

    if (dx == 0 && dy == 0) {
        std::cout << "Enemy at (" << x() << "," << y() << ") bites player for " << damage_ << "\n";
        player->takeDamage(damage_);
        return;
    }

    int sx = sign(dx);
    int sy = sign(dy);

    int nx = x() + sx;
    int ny = y() + sy;

    if (nx == px && ny == py) {
        std::cout << "Enemy at (" << x() << "," << y() << ") tries to step on player and deals " << damage_ << " dmg\n";
        player->takeDamage(damage_);
        return;
    }

    // проверяем границы и занятость
    if (field.isInside(nx, ny) && !field.unitAt(nx, ny) && !field.buildingAt(nx, ny)) {
        setPosition(nx, ny);
        std::cout << "Enemy moved to (" << nx << "," << ny << ")\n";
    } else {
        // иначе — не двигаемся
    }
}
