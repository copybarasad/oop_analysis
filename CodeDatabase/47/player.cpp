#include "player.h"
#include <cmath>
#include "game.h"

Player::Player(int startX, int startY) 
    : maxHealth(BASE_MAX_HEALTH),
      health(BASE_MAX_HEALTH),
      meleeDamage(2),
      rangedDamage(1),
      score(0),
      positionX(startX),
      positionY(startY),
      combatMode(CombatMode::MELEE),
      spellHand(3) {

    spellHand.addRandomSpell();
}

void Player::setHealth(int newHealth) {
    health = newHealth;
    if (health > maxHealth) {
        health = maxHealth;
    }
    if (health < 0) {
        health = 0;
    }
}

void Player::restoreFullHealth() {
    health = maxHealth;
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

int Player::getDamage() const {
    if (combatMode == CombatMode::MELEE) {
        return meleeDamage;
    } 
    else {
        return rangedDamage;
    }
}

int Player::getMeleeDamage() const {
    return meleeDamage;
}

int Player::getRangedDamage() const {
    return rangedDamage;
}

int Player::getScore() const {
    return score;
}

int Player::getPositionX() const {
    return positionX;
}

int Player::getPositionY() const {
    return positionY;
}

CombatMode Player::getCombatMode() const {
    return combatMode;
}

void Player::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) {
        health = 0;
    }
}

void Player::heal() {
    health += HEAL_AMOUNT;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Player::addScore(int points) {
    score += points;
    if (score > 1000000) {
        score = 1000000;
    }
}

void Player::move(int deltaX, int deltaY) {
    positionX += deltaX;
    positionY += deltaY;
}

void Player::switchCombatMode() {
    if (combatMode == CombatMode::MELEE) {
        combatMode = CombatMode::RANGED;
    } 
    else {
        combatMode = CombatMode::MELEE;
    }
}

bool Player::canAttack(int targetX, int targetY) const {
    bool sameX = (targetX == positionX);
    bool sameY = (targetY == positionY);
    
    if (!sameX && !sameY) {
        return false;
    }
    
    int distance = 0;
    if (sameX) {
        distance = std::abs(targetY - positionY);
    } 
    else {
        distance = std::abs(targetX - positionX);
    }
    
    if (combatMode == CombatMode::MELEE) {
        return distance == 1;
    }
    else {
        return distance <= RANGED_ATTACK_RANGE && distance > 0;
    }
}

bool Player::isAlive() const {
    return health > 0;
}

SpellHand& Player::getSpellHand() {
    return spellHand;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

void Player::increaseMaxHealth(int amount) {
    maxHealth += amount;
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Player::increaseMeleeDamage(int amount) {
    meleeDamage += amount;
}

void Player::increaseRangedDamage(int amount) {
    rangedDamage += amount;
}

void Player::increaseManaCapacity(int amount) {
    spellHand.increaseMaxMana(amount);
}

void Player::enhanceSpellPower() {
    spellHand.enhanceAllSpells();
}