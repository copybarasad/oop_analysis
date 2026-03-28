#include "player.h"
#include <iostream>
#include "game.h"

Player::Player(int hp, int dmg, size_t handSize) 
    : Entity(hp, dmg, "Player"), isMeleeMode(true), baseDamage(dmg), score(0), 
      mana(100), maxMana(100), spellHand(handSize) {
    damage = baseDamage * 2;
}

// Геттеры
bool Player::getCombatMode() const { return isMeleeMode; }
int Player::getScore() const { return score; }
int Player::getMana() const { return mana; }
int Player::getMaxMana() const { return maxMana; }
SpellHand& Player::getSpellHand() { return spellHand; }
const SpellHand& Player::getSpellHand() const { return spellHand; }

// Сеттеры для сохранения/загрузки
void Player::setMana(int newMana) {
    if (newMana < 0) throw std::invalid_argument("Mana cannot be negative");
    if (newMana > maxMana) throw std::invalid_argument("Mana cannot exceed max mana");
    mana = newMana;
}

void Player::setScore(int newScore) {
    if (newScore < 0) throw std::invalid_argument("Score cannot be negative");
    score = newScore;
}

void Player::setHealth(int newHealth) {
    if (newHealth < 0) throw std::invalid_argument("Health cannot be negative");
    if (newHealth > getMaxHealth()) throw std::invalid_argument("Health cannot exceed max health");
    health = newHealth;
}

void Player::setCombatMode(bool mode) {
    isMeleeMode = mode;
    damage = isMeleeMode ? baseDamage * 2 : baseDamage;
}

void Player::switchCombatMode() {
    isMeleeMode = !isMeleeMode;
    damage = isMeleeMode ? baseDamage * 2 : baseDamage;
    std::cout << "Switched to " << (isMeleeMode ? "MELEE" : "RANGED") << " combat mode" << std::endl;
}

void Player::addScore(int points) {
    if (points < 0) throw std::invalid_argument("Score points cannot be negative");
    score += points;
}

bool Player::castSpell(int spellIndex, Game& game, int targetX, int targetY) {
    auto spell = spellHand.getSpell(spellIndex);
    if (!spell) {
        return false;
    }
    
    if (mana < spell->getManaCost()) {
        std::cout << "Not enough mana to cast " << spell->getName() << "!" << std::endl;
        return false;
    }
    
    // Вычитаем ману
    mana -= spell->getManaCost();
    
    // Кастуем заклинание
    spell->cast(game, targetX, targetY);
    return true;
}

void Player::restoreMana(int amount) {
    if (amount < 0) throw std::invalid_argument("Mana restore amount cannot be negative");
    mana = std::min(mana + amount, maxMana);
}