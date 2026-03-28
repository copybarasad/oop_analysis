#include "Enemy.h"

Enemy::Enemy(int damage, Coordinates position, int maxHealth)
    : Being(maxHealth, maxHealth, damage, position) {}

json Enemy::toJson() const {
    return serializeBase();
}

void Enemy::fromJson(const json& j) {
    deserializeBase(j);
}