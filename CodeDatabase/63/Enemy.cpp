#include "Enemy.h"

Enemy::Enemy(unsigned int h, unsigned int d, unsigned int viewDiameter) : Entity(h, d, EntityTeam::ENEMY, 'E', viewDiameter, 0) {
    setPoints(h);
}

Enemy::Enemy(json &data) : Entity(data, EntityTeam::ENEMY) {}

std::unique_ptr<Entity> Enemy::clone() const {
    return std::make_unique<Enemy>(*this);
}
