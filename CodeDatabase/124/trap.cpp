#include "trap.h"
#include "enemy.h"  
#include <iostream>

Trap::Trap(int trapDamage, int trapDuration) 
    : damage(trapDamage), duration(trapDuration), isActive(false), x(-1), y(-1) {}

int Trap::getDamage() const { return damage; }
int Trap::getDuration() const { return duration; }
bool Trap::getIsActive() const { return isActive; }
void Trap::getPosition(int& outX, int& outY) const { outX = x; outY = y; }

void Trap::place(int posX, int posY) {
    x = posX;
    y = posY;
    isActive = true;
    std::cout << "Ловушка установлена в (" << x << "," << y << ")!\n";
}

bool Trap::checkTrigger(int enemyX, int enemyY, Enemy& enemy) {
    if (!isActive) return false;
    
    if (enemyX == x && enemyY == y) {
        enemy.takeDamage(damage);
        std::cout << "Враг наступил на ловушку! " << damage << " урона!\n";
        remove();
        return true;
    }
    return false;
}

void Trap::update() {
    if (isActive) {
        duration--;
        if (duration <= 0) {
            std::cout << "Ловушка в (" << x << "," << y << ") исчезла.\n";
            remove();
        }
    }
}

void Trap::remove() {
    isActive = false;
    x = -1;
    y = -1;
}