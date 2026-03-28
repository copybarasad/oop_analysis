#include "enemytower.h"
#include "field.h"
EnemyTower::EnemyTower(int x, int y) : canAttack(true), dmg(3) {
  this->x = x;
  this->y = y;
  this->lives = 100;
  this->range = 1;
  this->damage = 3;
}

void EnemyTower::resetAttack() { EnemyTower::canAttack = true; }

void EnemyTower::tryAttack(Player &player, Field &field,
                           DirectDamage &attackSpell, Logger &l) {

  int px = player.getX();
  int py = player.getY();

  // Manhattan distance (simpler, more consistent)
  int distance = std::abs((int)(px - x)) + std::abs((int)(py - y));

  if (canAttack && distance <= (int)range) {
    dmg.doDamage(player);
    l.logEvent(Events::TOWER_ATTACKED, NULL);
    canAttack = false; // skip next round
  }
}
