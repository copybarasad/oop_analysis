#include "EnemyTower.h"
#include "Player.h"
#include "GameField.h"
#include <algorithm>
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int x, int y, int hp, int damage, int range, int cooldownTurns)
        : Building(x, y, hp, true), // предполагается Building(int x,int y,int hp,bool isEnemy)
          damage_(damage),
          range_(range),
          cooldownTurns_(std::max(0, cooldownTurns)),
          cooldownCounter_(0)
{
}

bool EnemyTower::tryAttack(Player& player, GameField& /*field*/) {
    // Если в перезарядке — не атакуем
    if (cooldownCounter_ > 0) return false;

    int dx = std::abs(player.x() - x());
    int dy = std::abs(player.y() - y());
    int dist = std::max(dx, dy); // Chebyshev distance

    if (dist <= range_) {
        // ослабленная версия прямого урона
        int dmg = std::max(1, damage_ / 2);
        std::cout << "EnemyTower at (" << x() << "," << y() << ") attacks player for " << dmg << "\n";
        player.takeDamage(dmg);

        // запускаем перезарядку: после атаки ждем cooldownTurns_ ходов
        cooldownCounter_ = cooldownTurns_;
        return true;
    }
    return false;
}

void EnemyTower::tick() {
    if (cooldownCounter_ > 0) --cooldownCounter_;
}
