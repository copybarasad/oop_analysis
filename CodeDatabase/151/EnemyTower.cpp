#include "EnemyTower.h"
#include "GameField.h"

EnemyTower::EnemyTower(int x, int y, int attackRange)
    : position(x, y), attackRange(attackRange), cooldown(2), currentCooldown(0), canAttack(true), damage(15) {
}

void EnemyTower::update(GameField* field) {
    if (!canAttack) {
        currentCooldown--;
        if (currentCooldown <= 0) {
            canAttack = true;
        }
        return;
    }

    // »щем игрока в радиусе атаки
    Position playerPos = field->getPlayer()->getPosition();
    if (isInRange(playerPos)) {
        attack(field, playerPos);
        canAttack = false;
        currentCooldown = cooldown;
    }
}

Position EnemyTower::getPosition() const {
    return position;
}

bool EnemyTower::isInRange(const Position& target) const {
    int dx = std::abs(position.x - target.x);
    int dy = std::abs(position.y - target.y);
    return dx <= attackRange && dy <= attackRange;
}

void EnemyTower::attack(GameField* field, const Position& target) {
    int damage = 15; // базовый урон башни
    field->applyDamageToPosition(target, damage);
}