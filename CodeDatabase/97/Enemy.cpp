#include "Enemy.h"

Enemy::Enemy(int hp, int dmg) : Entity(hp, dmg, "Enemy") {}

std::shared_ptr<Entity> Enemy::clone() const { return std::make_shared<Enemy>(*this); }
