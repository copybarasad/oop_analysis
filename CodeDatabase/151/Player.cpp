#include "Player.h"
#include "GameField.h"
#include <iostream>

Player::Player(int startX, int startY, int health, int mana, int damage)
    : health(health), maxHealth(health), damage(damage), score(0),
    position(startX, startY), hand(5), mana(mana), maxMana(mana) {
    std::cout << "[DEBUG] Player constructed: HP=" << health
        << "  Mana=" << mana << "/100  Damage=" << damage << std::endl;
}

int Player::getHealth() const {
    return health;
}

int Player::getDamage() const {
    return damage;
}

int Player::getScore() const {
    return score;
}

Position Player::getPosition() const {
    return position;
}

void Player::setPosition(const Position& newPos) {
    position = newPos;
}

void Player::addScore(int points) {
    score += points;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::move(int dx, int dy) {
    position.x += dx;
    position.y += dy;
}

PlayerHand& Player::getHand() {
    return hand;
}

int Player::getMana() const {
    return mana;
}

int Player::getPendingUpgradesCount() const {
    return pendingUpgrades.size();
}

bool Player::castSpell(int spellIndex, const Position& targetPos, GameField* field) {
    if (!canCastSpell(spellIndex)) {
        return false;
    }

    Spell* spell = hand.getSpell(spellIndex);
    if (spell && mana >= spell->getManaCost()) {
        int upgradeCount = consumeUpgrades();
        if (upgradeCount > 0) {
            std::cout << "Applying " << upgradeCount << " upgrade(s)!\n";
        }

        bool success = spell->cast(field, position, targetPos);
        if (success) {
            mana -= spell->getManaCost();
            std::cout << "Successfully cast " << spell->getName() << "!\n";
        }
        else {
            std::cout << "Spell failed!\n";
        }
        return success;
    }
    return false;
}

void Player::addMana(int amount) {
    mana += amount;
    if (mana > maxMana) mana = maxMana;
}

bool Player::canCastSpell(int spellIndex) const {
    Spell* spell = hand.getSpell(spellIndex);
    return spell && mana >= spell->getManaCost();
}

void Player::addUpgrade(int strength) {
    pendingUpgrades.push_back(strength);
}

int Player::consumeUpgrades() {
    int total = 0;
    for (int upgrade : pendingUpgrades) {
        total += upgrade;
    }
    pendingUpgrades.clear();
    return total;
}

void Player::increaseDamage(int amount) {
    damage += amount;
    std::cout << "Player damage increased by " << amount << "!\n";
}