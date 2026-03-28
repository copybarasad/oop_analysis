#include "CombatSystem.h"

void CombatSystem::playerAttack() {
  int attackRange = (player.getCombatMode() == CombatMode::melee) ? 1 : 2;
  for (const auto &enemy : enemyManager.getInRange(player, attackRange)) {
    player.attack(*enemy);
  }
  for (const auto &building : buildingManager.getInRange(player, attackRange)) {
    player.attack(*building);
  }
}

void CombatSystem::enemyAttack() {
  for (const auto &enemy : enemyManager.getAlive()) {
    if (enemy->wasAdjacentToPlayer()) {
      enemy->attack(player);
    }
  }
}

int CombatSystem::cleanup() {
  int beforeEnemies = static_cast<int>(enemyManager.size());
  enemyManager.removeDead();
  int afterEnemies = static_cast<int>(enemyManager.size());
  buildingManager.removeDead();
  int defeated = beforeEnemies - afterEnemies;
  if (defeated < 0)
    defeated = 0;
  return defeated;
}