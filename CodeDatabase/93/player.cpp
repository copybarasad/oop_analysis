#include "player.h"
#include "direct_damage_spell.h"
#include "aoe_spell.h"
#include "trap_spell.h"
#include <random>
#include "save_manager.h"

Player::Player(int startX, int startY, int health)
    : position(startX, startY), health(health), maxHealth(health),
      meleeDamage(15), rangedDamage(8), score(0),
      combatMode(CombatMode::MELEE),
      hand_(3) {
    hand_.addRandomSpell();
}

Position Player::getPosition() const { return position; }
int Player::getHealth() const { return health; }
int Player::getMaxHealth() const { return maxHealth; }
int Player::getScore() const { return score; }
CombatMode Player::getCombatMode() const { return combatMode; }

int Player::getDamage() const {
    return (combatMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

int Player::getAttackRange() const {
    return (combatMode == CombatMode::MELEE) ? meleeRange : rangedRange;
}

void Player::move(int deltaX, int deltaY) {
    position.x += deltaX;
    position.y += deltaY;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::addScore(int points) {
    score += points;
}

void Player::switchCombatMode() {
    combatMode = (combatMode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}

bool Player::isAlive() const {
    return health > 0;
}

PlayerHand& Player::getHand() { return hand_; }
const PlayerHand& Player::getHand() const { return hand_; }

void Player::buySpell(int cost) {
    if (score < cost) return;
    hand_.addRandomSpell();
    score -= cost;
}

void Player::save(std::ostream& os) const {
    os << position.x << " " << position.y << " "
       << health << " " << maxHealth << " "
       << meleeDamage << " " << rangedDamage << " "
       << score << " " << static_cast<int>(combatMode) << " "
       << level_ << "\n";
    hand_.save(os);
}

void Player::load(std::istream& is) {
    int combatModeInt;
    is >> position.x >> position.y >> health >> maxHealth
       >> meleeDamage >> rangedDamage >> score >> combatModeInt >> level_;
    if (is.fail()) throw SaveDataException("Failed to read player data");
    combatMode = static_cast<CombatMode>(combatModeInt);
    std::string leftover;
    std::getline(is, leftover);
    hand_.load(is);
}

void Player::upgradeMaxHealth(int delta) {
    maxHealth += delta;
    health = maxHealth;
}

void Player::upgradeMeleeDamage(int delta) {
    meleeDamage += delta;
}

void Player::upgradeRangedDamage(int delta) {
    rangedDamage += delta;
}