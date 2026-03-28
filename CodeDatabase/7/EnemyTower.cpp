#include "EnemyTower.h"
#include "Player.h"
#include <iostream>
#include <cmath>
#include <algorithm>

EnemyTower::EnemyTower(int posX, int posY, int range, int damage, int cooldown)
    : x(posX), y(posY), range(range), damage(damage), cooldown(cooldown), currentCooldown(0) {}

void EnemyTower::update(Player& player) {
    if (currentCooldown > 0) {
        currentCooldown--;
        return;
    }

    if (player.isAlive()) {
        int distance = std::max(std::abs(x - player.getX()), std::abs(y - player.getY()));

        if (distance <= range) {
            std::cout << "Вражеская башня стреляет в " << player.getName() << "!" << std::endl;
            player.takeDamage(damage);
            currentCooldown = cooldown;
        }
    }
}

int EnemyTower::getX() const { return x; }
int EnemyTower::getY() const { return y; }