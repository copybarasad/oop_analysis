#include "enemy.h"

Enemy::Enemy(int id, int health, int damage): Entity(id, health), damage_(damage) {}
EntityType Enemy::getType() const { return EntityType::Enemy; }
int Enemy::getDamage() const { return damage_; }

Enemy* Enemy::asEnemy() { return this; }

#include <cmath>
std::pair<int,int> Enemy::chooseStep(int pr, int pc, int er, int ec) const {
    int dr = pr - er;
    int dc = pc - ec;
    int stepR = (dr==0)?0:(dr>0?1:-1);
    int stepC = (dc==0)?0:(dc>0?1:-1);
    if (std::abs(dr) >= std::abs(dc)) return {stepR, 0};
    return {0, stepC};
}
