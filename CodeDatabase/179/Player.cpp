#include "Player.h"

Player::Player(int health, int meleeDmg, int rangedDmg, int mana)
    : GameObject(health, "Player"), score(0), baseDamage(0),
      currentMode(FightMode::MELEE), meleeDamage(meleeDmg),
      rangedDamage(rangedDmg), mana(mana), maxMana(mana),
      rangedAttackRadius(3), skipNextTurn(false) {}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
}

FightMode Player::getFightMode() const {
    return currentMode;
}

void Player::switchFightMode() {
    currentMode = (currentMode == FightMode::MELEE) ?
                  FightMode::RANGED : FightMode::MELEE;
}

int Player::getDamage() const {
    return (currentMode == FightMode::MELEE) ? meleeDamage : rangedDamage;
}

int Player::getMeleeDamage() const {
    return meleeDamage;
}

int Player::getRangedDamage() const {
    return rangedDamage;
}

int Player::getMana() const {
    return mana;
}

int Player::getMaxMana() const {
    return maxMana;
}

void Player::setMana(int newMana) {
    mana = newMana;
    if (mana > maxMana) {
        mana = maxMana;
    }
    if (mana < 0) {
        mana = 0;
    }
}

// ДОБАВЛЯЕМ ЭТОТ МЕТОД
void Player::setMaxMana(int newMaxMana) {
    maxMana = newMaxMana;
    if (mana > maxMana) {
        mana = maxMana;
    }
}

bool Player::useMana(int amount) {
    if (mana >= amount) {
        mana -= amount;
        return true;
    }
    return false;
}

void Player::restoreMana(int amount) {
    mana += amount;
    if (mana > maxMana) {
        mana = maxMana;
    }
}

std::string Player::getType() const {
    return "Player";
}

int Player::getRangedAttackRadius() const {
    return rangedAttackRadius;
}

bool Player::shouldSkipTurn() const {
    return skipNextTurn;
}

void Player::setSkipTurn(bool skip) {
    skipNextTurn = skip;
}

void Player::resetTurnSkip() {
    skipNextTurn = false;
}

// Новые методы для прокачки
void Player::setMeleeDamage(int damage) {
    if (damage > 0) {
        meleeDamage = damage;
    }
}

void Player::setRangedDamage(int damage) {
    if (damage > 0) {
        rangedDamage = damage;
    }
}

void Player::setMaxHealth(int newMaxHealth) {
    if (newMaxHealth > 0) {
        maxHealth = newMaxHealth;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }
}
