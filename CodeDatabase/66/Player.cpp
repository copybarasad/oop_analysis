#include "../include/Player.h"

Player::Player(int health, int meleeDamage, int rangedDamage) 
    : health(health), maxHealth(health), meleeDamage(meleeDamage), rangedDamage(rangedDamage), 
      score(0), combatMode(CombatMode::MELEE), isSlowed(false) {}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

int Player::getCurrentDamage() const {
    return combatMode == CombatMode::MELEE ? meleeDamage : rangedDamage;
}

int Player::getScore() const {
    return score;
}

CombatMode Player::getCombatMode() const {
    return combatMode;
}

bool Player::getIsSlowed() const {
    return isSlowed;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Player::addScore(int points) {
    score += points;
}

void Player::switchMode() {
    combatMode = (combatMode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}

void Player::setSlow(bool slowed) {
    isSlowed = slowed;
}

bool Player::isAlive() const {
    return health > 0;
}

int Player::getMeleeDamage() const {
    return meleeDamage;
}

int Player::getRangedDamage() const {
    return rangedDamage;
}
