#include "enemyTower.h"
#include <cmath>

EnemyTower::EnemyTower(int startX, int startY, int initialHealth, int cooldownTuns)
  : x(startX), y(startY), health(initialHealth), cooldown(cooldownTuns), currentCooldown(0) {}

int EnemyTower::getX() const {return x;}
int EnemyTower::getY() const {return y;}
int EnemyTower::getHealth() const {return health;}
int EnemyTower::getCurrentCooldown() const {return currentCooldown;}
bool EnemyTower::isAlive() const {return health>0;}

void EnemyTower::takeDamage(int damage) {
  health -= damage;
  if (health<0) health = 0;
}

bool EnemyTower::update(int playerX, int playerY) {
  if (!isAlive()) return false;

  currentCooldown--;
  if (currentCooldown>0) return false;

  int distance = std::abs(playerX-x) + std::abs(playerY-y);
  if (distance<=3) {
    currentCooldown = cooldown;
    return true;
  }
  return false;
} 