#include "Enemy.h"

Enemy::Enemy(const std::string& playerName, int startX, int startY, int health)
    : Entity(playerName, startX, startY, health), damage(10) {}
