#include "Player.h"
#include "GameField.h"
#include "Unit.h"
#include "Building.h"

#include <iostream>
#include <cstdlib>

Player::Player(int x, int y, GameField& field, size_t handCapacity)
        : x_(x), y_(y), hasMoved_(false), hand_(handCapacity), field_(field) {}

void Player::moveTo(int nx, int ny) {
    if (hasMoved_) {
        std::cout << "Player: уже потратил ход.\n";
        return;
    }
    if (!field_.isInside(nx, ny)) {
        std::cout << "Player: за пределами поля.\n";
        return;
    }

    // Ограничение по расстоянию: один шаг (Chebyshev)
    int dx = std::abs(nx - x_);
    int dy = std::abs(ny - y_);
    if (std::max(dx, dy) > 1) {
        std::cout << "Player: слишком далеко (разрешён только 1 шаг).\n";
        return;
    }

    // Если в целевой клетке есть вражеский юнит -> атаковать (не перемещаясь).
    // Если атака убьёт юнита, то игрок перейдёт в ту клетку.
    Unit* targetUnit = field_.unitAt(nx, ny);
    if (targetUnit && targetUnit->isEnemy()) {
        attack(targetUnit);   // наносит урон и пишет в консоль
        // трата хода при попытке атаки
        spendTurn();

        // Если юнит умер в результате атаки — переходим в цель
        if (targetUnit->isDead()) {
            // Перемещение игрока в только что освобождённую клетку
            x_ = nx;
            y_ = ny;
            std::cout << "Player moved to (" << x_ << "," << y_ << ") after killing the enemy.\n";
            // Проверяем ловушки на новой клетке (хотя юнит погиб, ловушка могла быть)
            field_.checkTrapTrigger(x_, y_);
        }
        return;
    }

    // Если в целевой клетке есть здание -> запрет перемещения
    Building* b = field_.buildingAt(nx, ny);
    if (b) {
        std::cout << "Player: клетка ("<<nx<<","<<ny<<") занята зданием. Нельзя переместиться.\n";
        return;
    }

    // иначе перемещаемся
    x_ = nx; y_ = ny;
    std::cout << "Player moved to ("<<x_<<","<<y_<<")\n";
    field_.checkTrapTrigger(x_, y_);
    spendTurn();
}

void Player::takeDamage(int d) {
    hp_ -= d;
    std::cout << "Player took " << d << " dmg. HP=" << hp_ << "\n";
    if (hp_ <= 0) {
        std::cout << "Player died.\n";
        // Игровой цикл обработает окончание игры; не вызываем exit() здесь.
    }
}

void Player::onEnemyKilled() {
    kills_++;
    score_ += 10;
    if (kills_ % 3 == 0) {
        hand_.addRandomSpell();
    }
}

void Player::attack(Unit* u) {
    if (!u) return;
    std::cout << "Player attacks unit at (" << u->x() << "," << u->y() << ") for " << damage_ << "\n";
    u->takeDamage(damage_);
}
