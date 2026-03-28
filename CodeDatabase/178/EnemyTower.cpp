#include "EnemyTower.h"

EnemyTower::EnemyTower(const Position &pos, int hp, int dmg, int rng) : position(pos), health(hp), damage(dmg), range(rng), cooldown(0) {}

const Position &EnemyTower::getPosition() const { return position; }
bool EnemyTower::isAlive() const { return health>0; }
void EnemyTower::takeDamage(int d) { health -= d; if (health<0) health=0; }
bool EnemyTower::canFire() const { return cooldown==0; }
void EnemyTower::setCooldown(int cd) { cooldown = cd; }
void EnemyTower::tickCooldown() { if (cooldown>0) --cooldown; }
int EnemyTower::getRange() const { return range; }
int EnemyTower::getDamage() const { return damage; }