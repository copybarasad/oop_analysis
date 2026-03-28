#include "Enemy.h"

Enemy::Enemy(const Position &startPosition, int health, int damage) : Character(startPosition, health, damage) {}

void Enemy::move(const Position &newPosition) { position = newPosition; }