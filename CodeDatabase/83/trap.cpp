#include "trap.h"
#include "enemy.h"
#include <iostream>

Trap::Trap(int x, int y, const std::string& spellName, int damage)
    : x(x), y(y), spellName(spellName), damage(damage), active(true) {}

bool Trap::checkTrigger(int enemyX, int enemyY, Enemy& enemy) {
    if (active && enemyX == x && enemyY == y) {
        std::cout << "!!!" << spellName << " trap triggered at (" << x << ", " << y << ")!\n";
        enemy.takeDamage(damage);
        std::cout << "Enemy took " << damage << " damage! HP: " << enemy.getHealth() << "\n";
        trigger();
        return true;
    }
    return false;
}