#include "Player.h"
#include <algorithm>

Player::Player(int health, int meleeDmg, int rangedDmg, int playerMana, size_t spellHandSize)
    : maxHealth(health), currentHealth(health), meleeDamage(meleeDmg),
      rangedDamage(rangedDmg), score(0), enemiesKilled(0), 
      mana(playerMana), maxMana(playerMana), currentCombatMode(CombatMode::MELEE),
      isSlowed(false), spellHand(std::make_unique<SpellHand>(spellHandSize)),
      enemiesForNextSpell(3) {}

int Player::getHealth() const {
    return currentHealth;
}

int Player::getDamage() const {
    return (currentCombatMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

int Player::getScore() const {
    return score;
}

int Player::getEnemiesKilled() const {
    return enemiesKilled;
}

int Player::getMana() const {
    return mana;
}

int Player::getMaxMana() const {
    return maxMana;
}

CombatMode Player::getCombatMode() const {
    return currentCombatMode;
}

bool Player::getIsSlowed() const {
    return isSlowed;
}

SpellHand* Player::getSpellHand() const {
    return spellHand.get();
}

SpellEnhancementManager& Player::getEnhancementManager() {
    return enhancementManager;
}

const SpellEnhancementManager& Player::getEnhancementManager() const {
    return enhancementManager;
}

int Player::getEnemiesForNextSpell() const {
    return enemiesForNextSpell;
}

void Player::takeDamage(int damage) {
    currentHealth -= damage;
    if (currentHealth < 0) {
        currentHealth = 0;
    }
}

void Player::addScore(int points) {
    score += points;
}

void Player::addEnemyKilled() {
    enemiesKilled++;
}

void Player::switchCombatMode() {
    currentCombatMode = (currentCombatMode == CombatMode::MELEE)
        ? CombatMode::RANGED
        : CombatMode::MELEE;
}

void Player::setSlowed(bool slowed) {
    isSlowed = slowed;
}

void Player::addMana(int amount) {
    mana = std::min(mana + amount, maxMana);
}

bool Player::useMana(int amount) {
    if (mana >= amount) {
        mana -= amount;
        return true;
    }
    return false;
}

void Player::regenerateMana() {
    addMana(2);
}

bool Player::isAlive() const {
    return currentHealth > 0;
}
