#include "player.h"
#include <stdexcept>

Player::Player(int startHealth, int startMeleeDamage, int startRangedDamage) 
    : health(startHealth), meleeDamage(startMeleeDamage), 
      rangedDamage(startRangedDamage), score(0), currentMode(CombatMode::MELEE), 
      position(0, 0), canMove(true), mana(100), maxMana(100), spellHand(5) {
    validateHealth();
    validateMana();
    spellHand.initializeWithRandomSpell();
}

void Player::validateHealth() {
    if (health <= 0) {
        health = 0;
    }
}

void Player::validateMana() {
    if (mana < 0) {
        mana = 0;
    }
    if (mana > maxMana) {
        mana = maxMana;
    }
}

int Player::getHealth() const {
    return health;
}

int Player::getDamage() const {
    return (currentMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

int Player::getScore() const {
    return score;
}

CombatMode Player::getCombatMode() const {
    return currentMode;
}

Position Player::getPosition() const {
    return position;
}

bool Player::getCanMove() const {
    return canMove;
}

int Player::getMana() const {
    return mana;
}

int Player::getMaxMana() const {
    return maxMana;
}

void Player::setPosition(const Position& newPosition) {
    position = newPosition;
}

void Player::setCanMove(bool movable) {
    canMove = movable;
}

void Player::setMana(int newMana) {
    mana = newMana;
    validateMana();
}

void Player::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    health -= damage;
    validateHealth();
}

void Player::addScore(int points) {
    if (points < 0) {
        throw std::invalid_argument("Score points cannot be negative");
    }
    score += points;
}

void Player::restoreMana(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Mana restore amount cannot be negative");
    }
    mana += amount;
    validateMana();
}

bool Player::canCastSpell(int manaCost) const {
    return mana >= manaCost;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

void Player::switchCombatMode() {
    currentMode = (currentMode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}

void Player::onEnemyDefeated() {
    spellHand.onEnemyDefeated();
    restoreMana(5);
}

bool Player::isAlive() const {
    return health > 0;
}