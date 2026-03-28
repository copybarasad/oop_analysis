#include "Player.h"
#include "SpellHand.h"
#include <iostream>

Player::Player(int initialHealth, int initialMeleeDamage, int initialRangedDamage, int x, int y, int spellHandSize)
    : health(initialHealth), meleeDamage(initialMeleeDamage), rangedDamage(initialRangedDamage),
      score(0), currentCombatMode(CombatMode::Melee), position(x, y),
      spellHand(spellHandSize) {}

int Player::getHealth() const {
    return health;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Player::isAlive() const {
    return health > 0;
}

int Player::getDamage() const {
    return (currentCombatMode == CombatMode::Melee) ? meleeDamage : rangedDamage;
}

void Player::switchCombatMode() {
    if (currentCombatMode == CombatMode::Melee) {
        currentCombatMode = CombatMode::Ranged;
        std::cout << "Switched to Ranged mode (10 dmg, range 2)" << std::endl;
    } else {
        currentCombatMode = CombatMode::Melee;
        std::cout << "Switched to Melee mode (30 dmg, adjacent)" << std::endl;
    }
}

void Player::setCurrentCombatMode(CombatMode mode) {
    currentCombatMode = mode;
}

CombatMode Player::getCombatMode() const {
    return currentCombatMode;
}

std::pair<int, int> Player::getPosition() const {
    return position;
}

void Player::setPosition(int x, int y) {
    position.first = x;
    position.second = y;
}

void Player::addScore(int points) {
    score += points;
    if (score % 200 == 0 && score > 0) {
        gainSpell();
    }
}

void Player::setScore(int points) {
    score = points;
}

int Player::getScore() const {
    return score;
}

SpellHand& Player::getSpellHand() {
    return spellHand;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

void Player::gainSpell() {
    spellHand.refillRandomSpell();
    std::cout << "You earned a new spell! Hand size: " << spellHand.size() << std::endl;
}

void Player::restoreHealth() {
    health = 100;
}

void Player::increaseMeleeDamage(int amount) {
    meleeDamage += amount;
}

void Player::increaseRangedDamage(int amount) {
    rangedDamage += amount;
}