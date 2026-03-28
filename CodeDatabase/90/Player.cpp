#include "Player.h"

Player::Player(int meleeDamage, int rangedDamage, int health, Position position)
    : Entity(meleeDamage, health, position), combatMode(CombatMode::melee),
      hand(5), meleeDamage(meleeDamage), rangedDamage(rangedDamage),
      slowed(false) {}

CombatMode Player::getCombatMode() const { return combatMode; }

void Player::switchCombatMode() {
  combatMode = (combatMode == CombatMode::melee) ? CombatMode::ranged
                                                 : CombatMode::melee;
}

int Player::getCurrentDamage() const {
  return (combatMode == CombatMode::melee) ? meleeDamage : rangedDamage;
}

std::string Player::getCombatModeString() const {
  return (combatMode == CombatMode::melee) ? "Melee" : "Ranged";
}

void Player::castSpell() {}

void Player::attack(Entity &target) const {
  target.takeDamage(getCurrentDamage());
}

bool Player::isSlowed() const { return slowed; }

void Player::setSlowed(bool value) { slowed = value; }

template <typename T>
void Player::attackInRange(const std::vector<std::unique_ptr<T>> &entities,
                           int range) const {
  for (const auto &entity : entities) {
    if (entity && entity->isAlive() && distanceTo(*entity) <= range) {
      attack(*entity);
    }
  }
}
