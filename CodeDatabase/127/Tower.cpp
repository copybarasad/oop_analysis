#include "Tower.h"

Tower::Tower(int x, int y, int damage, int range, int hp, int cooldown)
    : Unit(x, y, hp, damage),
    range_(range),
    lastAttackTurn_(-1000),
    attackCooldown_(cooldown) {
}

int Tower::getRange() const { return range_; }
int Tower::getLastAttackTurn() const { return lastAttackTurn_; }
void Tower::setLastAttackTurn(int t) { lastAttackTurn_ = t; }
int Tower::getAttackCooldown() const { return attackCooldown_; }
