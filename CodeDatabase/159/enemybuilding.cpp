#include "enemybuilding.h"

EnemyBuilding::EnemyBuilding(int startX, int startY, int timer, int initialHealth)
  : x(startX), y(startY), timer(timer), currentTimer(timer), health(initialHealth) {}

int EnemyBuilding::getX() const {return x;}
int EnemyBuilding::getY() const {return y;}
int EnemyBuilding::getHealth() const {return health;}
int EnemyBuilding::getCurrentTimer() const {return currentTimer;}

bool EnemyBuilding::update() {
  if (!isAlive()) return false;
  currentTimer--;
  if (currentTimer<=0) {
    currentTimer = timer;
    return true;
  }
  return false;
}

void EnemyBuilding::takeDamage(int damage) {
  health -= damage;
  if (health<0) health = 0;
}

bool EnemyBuilding::isAlive() const {
  return health>0;
}